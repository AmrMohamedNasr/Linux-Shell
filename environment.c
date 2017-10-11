#include "environment.h"
#include "variables.h"
#include "shell_constants.h"
#include "limits.h"
#include <unistd.h>
#include <stdio.h>

/**
    - Registers a key,value pair as a variable in the variables table.
    - Text is entered as a string 'key=value'.
    @text
    - The string containing the key value under the format "key=value".
**/
void register_key_value(const char * text);
// Implementation(Documented in headers).
void setup_environment( char ** envp )
{
	// Initialize the variables table.
	initialize_variable_table();
	// Register all enviromental tables.
    int i = 0;
    for (i = 0; envp[i] != 0; i++) {
        register_key_value(envp[i]);
    }
    // Get current working directory to set a variable to it.
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != 0) {
        set_variable("HLD",cwd);
    } else {
        perror("Couldn't get current directory ");
    }
}
// Implementation (Documented in prototype).
void register_key_value(const char * text) {
    int i = 0;
    char key[STRING_MAX_SIZE], value[STRING_MAX_SIZE];
    int valueSwitch = 0;
    // Seperate strings to values and variables.
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
    // Set variable in the table.
    set_variable(key, value);
}
