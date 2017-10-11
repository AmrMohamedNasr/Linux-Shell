#include "commands.h"
#include "variables.h"
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "history.h"
#include "shell_constants.h"
#include <wordexp.h>

/**
    - Get the path of the home from the home variable.
    @path
    - Variable that will store the path in.
    @args
    - The arguments passed to the command.
**/
void go_home(char * path, char * const * args);


// Array of valid characters to start the variable and to be in the variable name generally.
static const char shell_variable_start_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
// Characters that can be part of the variables but not start them.
static const char shell_variable_chars[] = "0123456789";
// Previous directory.
static char previous_dir[PATH_MAX];

// Implementation (Documented in headers).
void history( char * const* args) {
    if (args[1] == NULL) {
        // If number of arguments is right, print history.
        print_history();
    } else {
        // Otherwise, print error message.
        fprintf(stderr, "Invalid number of arguments for history command...\n");
    }
}
// Implementation (Documented in headers).
void cd(  char* const* args )
{
    // Get current working directory.
    char cwd [PATH_MAX];
     if (getcwd(cwd, sizeof(cwd)) == NULL) {
       perror("Getting working directory error ");
    }
    // Begin getting targeted path.
    char * path = malloc(PATH_MAX * sizeof(char));
    if (args[1] == NULL) {
        // Go to home directory if no arguments are given.
        go_home(path, args);
        if (chdir(path) < 0) {
            perror("cd error ");
        } else {
            strcpy(previous_dir, cwd);
        }
    } else if (args[2] != NULL) {
        // More than two arguments, signify error.
        fprintf(stderr, "ERROR : cd error : too many arguments...\n");
    } else {
        // Check for the suitable directory.
        // If directory is ~ or ~/path
        if (args[1][0] == '-' && args[1][1] == '\0') {
            if (previous_dir[0] != '\0') {
                strcpy(path, previous_dir);
                printf("%s\n", path);
            }
        // If directory is specified realtive or absolute.
        } else {
            strcpy(path, args[1]);
        }
        // Change to the intended directory.
        if (chdir(path) < 0) {
            perror("cd error ");
        } else {
            strcpy(previous_dir, cwd);
        }
    }
    // Release resources.
    free(path);
}
// Implementation (Documented in prototype).
void go_home(char * path, char * const * args) {
    // Get home value.
    const char * result = lookup_variable("HOME", path);
    if (result == 0) {
        fprintf(stderr, "ERROR : cd error : no home variable found...\n");
    } else {
        int i = 1;
        int size = strlen(path);
        // connect the rest of the path to the home directory.
        if (args[1] != NULL) {
            while (args[1][i] != '\0') {
                path[size] = args[1][i];
                size++;
                i++;
            }
        }
        path[size] = '\0';
    }
}
// Implementation (Documented in headers).
void export_var( char* const* args ) {
    char temp[STRING_MAX_SIZE];
    if (args[2] == NULL) {
        char * equal_at = strchr(args[1], '=');
        if (equal_at == NULL) {
            if (valid_shell_name(args[1])) {
                const char * result = lookup_variable(args[1], temp);
                if (result == NULL) {
                    add_local_variable(args[1], "");
                    setenv(args[1], "", 1);
                } else {
                     setenv(args[1], temp, 1);
                }
            } else {
                fprintf(stderr, "EXPORT : error : Invalid variable name.. Should follow format[a-zA-Z_]+[a-zA-Z_0-9]*\n");
            }
        } else {
            strcpy(temp, equal_at + 1);
            * equal_at = '\0';
            if (valid_shell_name(args[1])) {
                add_local_variable(args[1], temp);
                setenv(args[1], temp, 1);
            } else {
                fprintf(stderr, "EXPORT : error : Invalid variable name.. Should follow format[a-zA-Z_]+[a-zA-Z_0-9]*\n");
            }
        }
    } else {
        fprintf(stderr, "EXPORT : error : Only supports exporting one variable at a time\n");
    }
}
// Implementation (Documented in headers).
int add_local_variable( char * key, char * value) {
    if (valid_shell_name(key) == 1) {
        set_variable(key, value);
        return 1;
    } else {
        return 0;
    }
}
// Implementation (Documented in headers).
void echo( char* const* args )
{
    // Loop over the arguments and print them with spaces between them.
    int i = 1;
    if (args[i] != NULL) {
        printf("%s", args[i]);
        i++;
    }
	while (args[i] != NULL) {
        printf(" %s", args[i]);
        i++;
	}
	printf("\n");
}
// Implementation (Documented in headers).
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
        i++;
    }
    return 1;
}
// Implementation (Documented in headers).
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
