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

typedef enum{ exit_command = 0 , change_dir_command = 1, echo_command = 2, general_command = 3, variable_command = 4, history_command = 5, comment = 6 } command_type ;

void getCorrectPath(char * path, const char * file);

int split_string_to_args(const char * command, char * const * args);
int valid_shell_name(const char * variable);
int valid_shell_char(const char c, int start);

static const char shell_variable_start_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
static const char shell_variable_chars[] = "0123456789";

int valid_shell_name(const char * variable) {
    int i = 0;
    if (valid_shell_char(variable[i], 1) == 0) {
        return 0;
    }
    i = 1;
    while (variable[i] != '\0') {
        if (valid_shell_char(variable[i], 0) == 0) {
            return 0;
        }
    }
    return 1;
}

int valid_shell_char(const char c, int start) {
    int i = 0;
    while (shell_variable_start_chars[i] != '\0') {
        if (shell_variable_start_chars[i] == c) {
            return 1;
        }
        i++;
    }
    if (start == 0) {
        i = 0;
        while (shell_variable_chars[i] != '\0') {
            if (shell_variable_chars[i] == c) {
                return 1;
            }
            i++;
        }
    }
    return 0;
}
int parse_command( const char* command, char ** args, int * commandType, int * background) {
    //int argsLen = split_string(command, args, delimiter);
    int argsLen = split_string_to_args(command, args);
    if (argsLen < 0) {
        return argsLen;
    }
    if (argsLen > 0 && strcmp(args[argsLen - 1], "&") == 0) {
        argsLen--;
        * background = 1;
    } else {
        * background = 0;
    }
    if (strcmp(args[0], "exit") == 0) {
        * commandType = exit_command;
    } else if (strcmp(args[0], "cd") == 0) {
        * commandType = change_dir_command;
    } else if (strcmp(args[0], "echo") == 0) {
        * commandType = echo_command;
    } else if (strcmp(args[0], "history") == 0) {
        * commandType = history_command;
    } else if (args[0][0] ==  '#')  {
        * commandType = comment;
    } else {
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
    free(args[argsLen]);
    args[argsLen] = NULL;
    return argsLen;
}

int split_string_to_args(const char * command, char * const * args) {
    char * copy = malloc(STRING_MAX_SIZE * sizeof(char));
    char * temp_buffer = malloc(STRING_MAX_SIZE * sizeof(char));
    char * temp_value = malloc(STRING_MAX_SIZE * sizeof(char));
    remove_leading_spaces(command, copy);
    int i = 0;
    int argsLen = 0;
    int mode = 0;
    if (copy[i] == '#') {
        strcpy(args[argsLen], copy);
        argsLen++;
        return argsLen;
    }
    int argsI = 0;
    while (copy[i] != '\0') {
         if (copy[i] == '#' && isSpace(copy[i - 1]) ) {
            break;
        }
        while((isSpace(copy[i]) == 0 || mode != 0) && copy[i] != '\0') {
            if (copy[i] == '"' && mode != 2) {
                mode = mode == 1 ? 0 : 1;
            } else if (copy[i] == '\'' && mode != 1) {
                mode = mode == 2 ? 0 : 2;
            } else if (copy[i] == '$' && valid_shell_char(copy[i + 1], 1) == 1) {
                i++;
                int buf_index = 0;
                while (valid_shell_char(copy[i] , 0) == 1) {
                    temp_buffer[buf_index] = copy[i];
                    i++;
                    buf_index++;
                }
                i--;
                temp_buffer[buf_index] = '\0';
                const char * result = lookup_variable((const char *)temp_buffer, temp_value);
                if (result != 0) {
                    args[argsLen][argsI] = '\0';
                    argsI += strlen(temp_value);
                    strcat(args[argsLen], temp_value);
                }
            } else {
                args[argsLen][argsI] = copy[i];
                argsI++;
            }
            i++;
        }
        args[argsLen][argsI] = '\0';
        if (strlen(args[argsLen]) != 0) {
            argsLen++;
        }
        argsI = 0;
        while(isSpace(copy[i]) == 1) {
            i++;
        }
    }
    free(copy);
    free(temp_buffer);
    free(temp_value);
    if (mode != 0) {
        return -1;
    }
    return argsLen;
}

int execute_command( char * const* args, const int * commandType, const int * background) {
    if (*commandType == exit_command) {
        return 0;
    } else if (*commandType == comment) {
        return 1;
    }
    char * path = malloc(PATH_MAX * sizeof(char));
    int pid;
    if (*background == 0) {
        switch (* commandType) {
            case change_dir_command:
                cd(args);
                break;
            case echo_command:
                echo(args);
                break;
            case general_command:
                pid = fork();
                if (pid < 0) {
                    perror("Fork failed ");
                } else if (pid == 0) {
                    getCorrectPath(path, args[0]);
                    if (execv(path, args) == -1) {
                        fprintf(stderr, "No matching command found...\n");
                        exit(1);
                    }
                } else {
                    int status;
                    waitpid(pid, &status, 0);
                }
                break;
            case history_command:
                history(args);
                break;
            case variable_command:
                if (valid_shell_name(args[0]) == 1) {
                    set_variable(args[0], args[1]);
                } else {
                    fprintf(stderr, "Error : invalid variable name, should be [a-zA-Z_][a-zA-Z_0-9]*\n");
                }
                break;
            default:
                fprintf(stderr, "Unknown command type....\n");
                break;
        }
    } else {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed ");
        } else if (pid == 0) {
            switch (* commandType) {
                case change_dir_command:
                    cd(args);
                    break;
                case echo_command:
                    echo(args);
                    break;
                case general_command:
                    getCorrectPath(path, args[0]);
                    if (execv(path, args) == -1) {
                        fprintf(stderr, "No matching command found...\n");
                    }
                    break;
                case history_command:
                    history(args);
                    break;
                case variable_command:
                    if (valid_shell_name(args[0]) == 1) {
                        set_variable(args[0], args[1]);
                    } else {
                        fprintf(stderr, "Error : invalid variable name, should be [a-zA-Z_][a-zA-Z_0-9]*\n");
                    }
                    break;
                default:
                    fprintf(stderr, "Unknown command type....\n");
                    break;
            }
            exit(0);
        }
    }
    free(path);
    return 1;
}

void getCorrectPath(char * path, const char * file) {
    char * paths = malloc(STRING_MAX_SIZE * sizeof(char));
    const char * result = lookup_variable("PATH", paths);
    if (result == 0) {
        strcpy(path, file);
    } else {
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
        } else if (file[0] == '/') {
            strcpy(path, file);
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
