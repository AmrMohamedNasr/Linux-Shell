#include "commands.h"
#include "variables.h"
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char previous_dir[PATH_MAX];

void cd(  char* const* args )
{
    char cwd [PATH_MAX];
     if (getcwd(cwd, sizeof(cwd)) == NULL) {
       perror("Getting working directory error ");
    }
    if (args[1] == NULL) {
        if (previous_dir[0] != '\0') {
            if (chdir(previous_dir) < 0) {
                perror("cd error :");
            } else {
                strcpy(previous_dir, cwd);
            }
        }
    } else if (args[2] != NULL) {
        fprintf(stderr, "ERROR : cd error : too many arguments...\n");
    } else {
        char * path = malloc(PATH_MAX * sizeof(char));
        if (args[1][0] == '~') {
            const char * result = lookup_variable("HOME", path);
            if (result == 0) {
                fprintf(stderr, "ERROR : cd error : no home variable found...\n");
            } else {
                int i = 1;
                int size = strlen(path);
                while (args[1][i] != '\0') {
                    path[size] = args[1][i];
                    size++;
                    i++;
                }
                path[size] = '\0';
            }
        } else {
            strcpy(path, args[1]);
        }
        if (chdir(path) < 0) {
            perror("cd error :");
        } else {
            strcpy(previous_dir, cwd);
        }
    }
}


void echo( const char * command,  char* const* args )
{
	// you should implement this function
	printf("ECHO\n");
}
