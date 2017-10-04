#include "environment.h"
#include "variables.h"
#include "shell_constants.h"
#include "limits.h"
#include <unistd.h>
#include <stdio.h>

void register_key_value(const char * text);

void setup_environment( char ** envp )
{
	initialize_variable_table();
    int i = 0;
    for (i = 0; envp[i] != 0; i++) {
        register_key_value(envp[i]);
    }
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != 0) {
        set_variable("HLD",cwd);
    } else {
        perror("Couldn't get current directory ");
    }
}

void register_key_value(const char * text) {
    int i = 0;
    char key[STRING_MAX_SIZE], value[STRING_MAX_SIZE];
    int valueSwitch = 0;
    while (text[i] != '\0') {
        if (valueSwitch == 0) {
            if (text[i] != '=') {
                key[i] = text[i];
            } else {
                valueSwitch = i + 1;
            }
        } else {
            value[i - valueSwitch] = text[i];
        }
        i++;
    }
    key[valueSwitch - 1] = '\0';
    value[i - valueSwitch] = '\0';
    set_variable(key, value);
}
