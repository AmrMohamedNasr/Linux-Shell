#include "commands.h"
#include "variables.h"
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "history.h"

void go_home(char * path, char * const * args);

static char previous_dir[PATH_MAX];

void history( char * const* args) {
    if (args[1] == NULL) {
        // If number of arguments is right, print history.
        print_history();
    } else {
        // Otherwise, print error message.
        fprintf(stderr, "Invalid number of arguments for history command...\n");
    }
}

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
            perror("cd error :");
        } else {
            strcpy(previous_dir, cwd);
        }
    } else if (args[2] != NULL) {
        // More than two arguments, signify error.
        fprintf(stderr, "ERROR : cd error : too many arguments...\n");
    } else {
        // Check for the suitable directory.
        // If directory is ~ or ~/path
        if (args[1][0] == '~' && (args[1][1] =='/' || args[1][1] =='\0')) {
            go_home(path, args);
        // If directory is ~username
        } else if (args[1][0] == '~') {
            int i = 1;
            strcpy(path, "/home/");
            int size = strlen(path);
            while (args[1][i] != '\0') {
                path[size] = args[1][i];
                size++;
                i++;
            }
            path[size] = '\0';
        // if directory is - and only - then swap directories.
        } else if (args[1][0] == '-' && args[1][1] == '\0') {
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
            perror("cd error :");
        } else {
            strcpy(previous_dir, cwd);
        }
    }
    // Release resources.
    free(path);
}

void go_home(char * path, char * const * args) {
    const char * result = lookup_variable("HOME", path);
    if (result == 0) {
        fprintf(stderr, "ERROR : cd error : no home variable found...\n");
    } else {
        int i = 1;
        int size = strlen(path);
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
void echo( char* const* args )
{
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
