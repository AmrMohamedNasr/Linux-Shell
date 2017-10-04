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

typedef enum{ exit_command = 0 , change_dir_command = 1, echo_command = 2, general_command = 3, variable_command = 4, history_command = 5, comment = 6 } command_type ;

int split_string(const char * message, char ** args, const char * delimiter);
void getCorrectPath(char * path, const char * file);

static const char delimiter[] = " \t\r\n\f\v";

int parse_command( const char* command, char ** args, int * commandType, int * background) {
    int argsLen = split_string(command, args, delimiter);
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
    } else if (args[0][0] ==  '#')  {
        * commandType = comment;
    } else {
        * commandType = general_command;
    }
    args[argsLen] = NULL;
    return argsLen;
}

int execute_command( const char * command, char * const* args, const int * commandType, const int * background) {
    if (*commandType == exit_command) {
        return 0;
    }
    char * path = malloc(PATH_MAX * sizeof(char));
    int pid;
    switch (* commandType) {
        case change_dir_command:
            cd(args);
            break;
        case echo_command:
            echo(command, args);
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
                if (* background == 0) {
                    int status;
                    waitpid(pid, &status, 0);
                }
            }
            break;
        case history_command:
            break;
        case variable_command:
            break;
        case comment:
            break;
        default:
            fprintf(stderr, "Unknown command type....\n");
            break;
    }
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
            char * token = strtok(paths,":");;
            while (access(path, F_OK ) == -1 && token != NULL) {
                strcpy(path, token);
                strcat(path, "/");
                strcat(path, file);
                token = strtok(NULL, ":");
            }
        }
    }
}

int split_string(const char * message, char ** args, const char * delimiter) {
    char * token;
    char * copy = strdup(message);
    token = strtok(copy, delimiter);
    int i = 0;
    while (token != NULL) {
        strcpy(args[i], token);
        token = strtok(NULL, delimiter);
        i++;
    }
    return i;
}
