#include "variables.h"
#include "commands.h"
#include "command_parser.h"
#include "environment.h"
#include "file_processing.h"
#include <stdlib.h>
#include <string.h>
#include "shell_constants.h"
#include "history.h"
#include "signal_handler.h"

typedef enum{ false = 0 , true = 1 } bool ;

void start_shell(bool read_from_file);
void shell_loop(bool input_from_file);

int main(int argc, char *argv[], char** envp)
{
    setup_environment(envp);

    // any other early configuration should be here

    intialize_history_table();
    start_signal_handlers();

    if(argc > 2) {
        fprintf(stderr, "Invalid number of arguments. Command should be entered as <shell [batch file]>\n");
    } else if( argc == 2 ){
        open_commands_batch_file(argv[1]);
        if (get_commands_batch_file() != NULL) {
            start_shell(true);
        }
    }
    else{
        start_shell(false);
    }

    return 0;
}

void start_shell(bool read_from_file)
{
    const char *init_directory [3] = {"cd", "~", NULL};
	cd((char * const *)init_directory);
	shell_loop(read_from_file);
}

void shell_loop(bool input_from_file)
{
	bool from_file = input_from_file;
    char * input = malloc((MAX_COMMAND_LEN ) * sizeof(char));
	char **args = malloc (sizeof (char *) * (MAX_COMMAND_LEN));
	char * eofDetector = 0;
	int * commandtype = malloc(sizeof(int));
	int * background = malloc(sizeof(int));
	int i = 0;
    int working = 1;
    for (i = 0; i < MAX_COMMAND_LEN; i++) {
        args[i] = malloc(sizeof(char) * MAX_COMMAND_LEN);
    }
    int file_counter = 0;
    while(working == 1) {
		printf("Shell>");
		if(from_file){
			//read next instruction from file
			fseek(get_commands_batch_file(), file_counter, SEEK_SET);
            eofDetector = fgets(input, (MAX_COMMAND_LEN), get_commands_batch_file());
            file_counter += strlen(input);
            // if end of file {from_file = false; continue;}
			if (eofDetector == 0) {
                from_file = false;
                close_commands_batch_file();
                printf("\n");
                continue;
			} else {
                printf("%s\n", input);
			}
		}
		else {
			//read next instruction from console
			eofDetector = fgets(input, (MAX_COMMAND_LEN + 1), stdin);
            if (eofDetector == 0) {
                break;
            }
		}
		if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
		}
		// parse your command here
        int nulled_index = parse_command(input, args, commandtype, background);
		// execute your command here

		if (nulled_index > 0) {
            add_to_history(input);
            working = execute_command(( char * const*)args, commandtype, background);
        } else if (nulled_index < 0) {
            fprintf(stderr, "Invalid use of quotes.. please enter a valid command\n");
        }
        // reallocating the nulled index element in the args list.
        if (nulled_index > -1) {
            args[nulled_index] = malloc(sizeof(char) * MAX_COMMAND_LEN);
        }
	}
	printf("\n");
	free(input);
	i = 0;
	for (i = 0; i < MAX_COMMAND_LEN; i++) {
        if (args[i] != 0) {
            free(args[i]);
        }
    }
    free(background);
    free(commandtype);
    free(args);
    clear_variable_table();
    clear_history_table();
    stop_signal_handlers();
}
