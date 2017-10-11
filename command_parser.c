#include "command_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "shell_constants.h"
#include "commands.h"
#include "variables.h"
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include "string_operations.h"

// Command types.
typedef enum{ exit_command = 0 , change_dir_command = 1, echo_command = 2, general_command = 3, variable_command = 4, history_command = 5, comment = 6, export_command = 7 } command_type ;

/**
    - Searchs for the correct path for any command.
    - If it finds the correct path it returns it.
    - Otherwise return a string containing the command.
    @path
    - The string containing the result of the operation.
    @file
    - The command name or the command executable file name more accuratly.
**/
void getCorrectPath(char * path, const char * file);
/**
    - Processes the command and return a list of arguments, where variables
    are replaced by their values and any other kind of preprocessing.
    @command
    - The command to parse.
    @args
    - The list of parameters return by the string.
    @return
    - The number of arguments.
**/
int split_string_to_args(const char * command, char * const * args);

// Implementation (Documented in headers).
int parse_command( const char* command, char ** args, int * commandType, int * background) {
    // Split the command to arguments.
    int argsLen = split_string_to_args(command, args);
    // If lower than 0, then there is a problem.
    if (argsLen < 0) {
        return argsLen;
    }
    // Check if its a background operation.
    if (argsLen > 0 && strcmp(args[argsLen - 1], "&") == 0) {
        argsLen--;
        * background = 1;
    } else if (args[argsLen - 1][strlen(args[argsLen - 1]) - 1] == '&') {
        * background = 1;
        args[argsLen - 1][strlen(args[argsLen - 1]) - 1] = '\0';
    } else {
        * background = 0;
    }
    // Check the command type.
    if (strcmp(args[0], "exit") == 0) {
        * commandType = exit_command;
    } else if (strcmp(args[0], "cd") == 0) {
        * commandType = change_dir_command;
    } else if (strcmp(args[0], "echo") == 0) {
        * commandType = echo_command;
    } else if (strcmp(args[0], "history") == 0) {
        * commandType = history_command;
    } else if (strcmp(args[0], "export") == 0) {
        * commandType = export_command;
    } else if (args[0][0] ==  '#')  {
        * commandType = comment;
    } else {
        // Check if assigment.
        if (argsLen == 1) {
            char * equal_start = strchr(args[0], '=');
            if (equal_start != NULL) {
                strcpy(args[argsLen], equal_start + 1);
                (* equal_start) = '\0';
                argsLen++;
                *commandType = variable_command;
            } else {
                * commandType = general_command;
            }
        } else {
            * commandType = general_command;
        }
    }
    // Free resources and ready the array to be given to the executing function.
    free(args[argsLen]);
    args[argsLen] = NULL;
    return argsLen;
}
// Implementation (Documented in headers).
int execute_command( char * const* args, const int * commandType, const int * background) {
    // Check if command type doesn't require many actions.
    if (*commandType == exit_command) {
        return 0;
    } else if (*commandType == comment) {
        return 1;
    }
    // Get ready to check if its background.
    // If background, fork always otherwise only fork commands depending on execv.
    char * path = malloc(PATH_MAX * sizeof(char));
    int pid;
    if (*background == 0) {
        // Do suitable action according to the command.
        switch (* commandType) {
            case change_dir_command:
                cd(args);
                break;
            case echo_command:
                echo(args);
                break;
            case general_command:
                pid = fork();
                // Fork to protect the parent.
                if (pid < 0) {
                    perror("Fork failed ");
                } else if (pid == 0) {
                    // Child : get right path and execv it.
                    getCorrectPath(path, args[0]);
                    if (execv(path, args) == -1) {
                        fprintf(stderr, "No matching command found...\n");
                        // close child process.
                        exit(1);
                    }
                } else {
                    int status;
                    // Wait for child, foreground operation...
                    waitpid(pid, &status, 0);
                }
                break;
            case history_command:
                history(args);
                break;
            case variable_command:
                //Check if its a valid variable name and set it if it is.
                if (add_local_variable(args[0], args[1]) == 0) {
                    fprintf(stderr, "Error : invalid variable name, should be [a-zA-Z_][a-zA-Z_0-9]*\n");
                }
                break;
            case export_command:
                //export arguments.
                export_var(args);
                break;
            default:
                fprintf(stderr, "Unknown command type....\n");
                break;
        }
    } else {
        // Background : Just fork.
        pid = fork();
        if (pid < 0) {
            perror("Fork failed ");
        } else if (pid == 0) {
            // Check command type.
            switch (* commandType) {
                case change_dir_command:
                    cd(args);
                    break;
                case echo_command:
                    echo(args);
                    break;
                case general_command:
                    // Get correct path and execute program.
                    getCorrectPath(path, args[0]);
                    if (execv(path, args) == -1) {
                        fprintf(stderr, "No matching command found...\n");
                    }
                    break;
                case history_command:
                    history(args);
                    break;
                case variable_command:
                    // Check if valid variable name and split it if it is.
                    if (add_local_variable(args[0], args[1]) == 0) {
                        fprintf(stderr, "Error : invalid variable name, should be [a-zA-Z_][a-zA-Z_0-9]*\n");
                    }
                    break;
                case export_command:
                    //export arguments.
                    export_var(args);
                    break;
                default:
                    fprintf(stderr, "Unknown command type....\n");
                    break;
            }
            // close child process.
            exit(0);
        }
    }
    // free resources.
    free(path);
    return 1;
}


// Implementation (DOcumented above prototype).
void getCorrectPath(char * path, const char * file) {
    // search for the correct path of a command.
    char * paths = malloc(STRING_MAX_SIZE * sizeof(char));
    // Get PATH variable value.
    const char * result = lookup_variable("PATH", paths);
    if (result == 0) {
        // If not found just copy the command name/path given to you.
        strcpy(path, file);
    } else {
        // If relative, just get working directory and add user input on it.
        if (file[0] == '.' && file[1] == '/') {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
               int i = 0;
               while (cwd[i] != '\0') {
                    path[i] = cwd[i];
                    i++;
               }
               int j = 1;
               while (file[j] != '\0') {
                    path[i] = file[j];
                    i++;
                    j++;
               }
               path[i] = '\0';
            } else {
               perror("Getting working directory error ");
            }
        // If absolute path is given, just use it.
        } else if (file[0] == '/') {
            strcpy(path, file);
        // Else, check for all possible places by using PATH to guess where our command exe file is.
        } else {
            char * token = strtok(paths,":");
            while (access(path, F_OK ) == -1 && token != NULL) {
                strcpy(path, token);
                strcat(path, "/");
                strcat(path, file);
                token = strtok(NULL, ":");
            }
        }
    }
    free(paths);
}

void replace_variables(char * output, const char * command) {
    int i = 0;
    int cur_i = 0;
    char * temp_buffer = malloc(STRING_MAX_SIZE * sizeof(char));
    char * temp_value = malloc(STRING_MAX_SIZE * sizeof(char));
    // Loop over command.
    while (command[i] != '\0') {
        // If a variable start, replace the variable with the result.
        if (command[i] == '$' && valid_shell_char(command[i + 1], 1) == 1) {
            i++;
            // Get all variable name in temp_buffer.
            int buf_index = 0;
            while (valid_shell_char(command[i] , 0) == 1) {
                temp_buffer[buf_index] = command[i];
                i++;
                buf_index++;
            }
            i--;
            temp_buffer[buf_index] = '\0';
            // Look up for its result.
            const char * result = lookup_variable((const char *)temp_buffer, temp_value);
            // If there is a value, add it to the output.
            if (result != 0) {
                output[cur_i] = '\0';
                strcat(output, temp_value);
                cur_i = strlen(output) - 1;
            }
        }
        // Else add the character.
        else {
            output[cur_i] = command[i];
        }
        i++;
        cur_i++;
    }
    output[cur_i] = '\0';
    free(temp_buffer);
    free(temp_value);
}
// Implementation (DOcumented above prototype).
int split_string_to_args(const char * command, char * const * args) {
    char * no_var_copy = malloc(STRING_MAX_SIZE * 4 * sizeof(char));
    char * copy = malloc(STRING_MAX_SIZE* 4 * sizeof(char));
    replace_variables(no_var_copy, command);
    // remove command leading spaces.
    remove_leading_spaces(no_var_copy, copy);
    int i = 0;
    int argsLen = 0;
    int mode = 0;
    // Check if its a comment
    // stop processing if it is.
    if (copy[i] == '#') {
        strcpy(args[argsLen], copy);
        argsLen++;
        free(copy);
        free(no_var_copy);
        return argsLen;
    }
    int argsI = 0;
    while (copy[i] != '\0') {
        // If the rest is a comment just break.
         if (copy[i] == '#' && isSpace(copy[i - 1]) ) {
            break;
        }
        // Parse an argument.
        while((isSpace(copy[i]) == 0 || mode != 0) && copy[i] != '\0') {
            // If quotes are found and it is not contained in a different type of quotes.
            if (copy[i] == '"' && mode != 2) {
                mode = mode == 1 ? 0 : 1;
            // Same as above.
            } else if (copy[i] == '\'' && mode != 1) {
                mode = mode == 2 ? 0 : 2;
            // Else add the character.
            } else {
                args[argsLen][argsI] = copy[i];
                argsI++;
            }
            i++;
        }
        // Close the arugment string.
        args[argsLen][argsI] = '\0';
        // Make sure not to add an empty argutment.
        if (strlen(args[argsLen]) != 0) {
            argsLen++;
        }
        argsI = 0;
        // Pass by white space characters.
        while(isSpace(copy[i]) == 1) {
            i++;
        }
    }
    free(copy);
    free(no_var_copy);
    // check if quotes are unmatching.
    if (mode != 0) {
        return -1;
    }
    return argsLen;
}
