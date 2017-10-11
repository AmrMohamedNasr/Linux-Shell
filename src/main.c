#include "../include/variables.h"
#include "../include/commands.h"
#include "../include/command_parser.h"
#include "../include/environment.h"
#include "../include/file_processing.h"
#include <stdlib.h>
#include <string.h>
#include "../include/shell_constants.h"
#include "../include/history.h"
#include "../include/signal_handler.h"
#include "../include/string_operations.h"

// Enum for true and false.
typedef enum{ false = 0 , true = 1 } bool ;
/**
    - Function responsible of early stages of starting the shell.
    @read_from_file
    - True if the shell should be running in batch mode.
**/
void start_shell(bool read_from_file);
/**
    - Function responsible of the shell working stage.
    @read_from_file
    - True if the shell should be starting in batch mode.
**/
void shell_loop(bool input_from_file);
/**
    - Main body of our shell.
    @argc
    - Number of arguments entered.
    @argv
    - List of arguments entered.
    @envp
    - List of enviromental variables.
**/
int main(int argc, char *argv[], char** envp)
{
    // Setup up our variables table and enviroment.
    setup_environment(envp);
    // Start up our history service.
    intialize_history_table();
    // Start up our signal handling service.
    start_signal_handlers();
    // Check number of arguments :
    int valid_args = 1;
    bool read_file_mode;
    // If more than 2, invalid number of arguments.
    if(argc > 2) {
        fprintf(stderr, "Invalid number of arguments. Command should be entered as <shell [batch file]>\n");
        valid_args = 0;
    // If 2 then try to run in batch mode.
    } else if( argc == 2 ){
        open_commands_batch_file(argv[1]);
        // Check if opening was successful.
        if (get_commands_batch_file() != NULL) {
            read_file_mode = true;
        } else {
            valid_args = 0;
        }
    }
    // Interactive mode.
    else{
        read_file_mode = false;
    }
    // If valid arguments were entered, run shell.
    if (valid_args == 1) {
        // Run shell in suitable mode.
        start_shell(read_file_mode);
    }
    // Free resources.
    clear_variable_table();
    clear_history_table();
    stop_signal_handlers();
    return 0;
}
// Implmentation (Documented above prototype).
void start_shell(bool read_from_file)
{
    // Move shell to home.
    const char *init_directory [2] = {"cd",NULL};
	cd((char * const *)init_directory);
	// start shell loop.
	shell_loop(read_from_file);
}
// Implmentation (Documented above prototype).
void shell_loop(bool input_from_file)
{
    // Allocate resources that will be used.
	bool from_file = input_from_file;
    char * input = malloc((MAX_COMMAND_LEN + 1) * sizeof(char));
    int input_size = MAX_COMMAND_LEN + 1;
	char **args = malloc (sizeof (char *) * (MAX_COMMAND_LEN));
	char * eofDetector = 0;
	int * commandtype = malloc(sizeof(int));
	int * background = malloc(sizeof(int));
	int i = 0;
    int working = 1;
    for (i = 0; i < MAX_COMMAND_LEN; i++) {
        args[i] = malloc(sizeof(char) * STRING_MAX_SIZE);
    }
    int file_counter = 0;
    // Working loop.
    while(working == 1) {
		// Check mode.
		if(from_file){
			//read next instruction from file
			fseek(get_commands_batch_file(), file_counter, SEEK_SET);
            eofDetector = read_line(&input, &input_size, get_commands_batch_file());
            file_counter += strlen(input);
            // if end of file {from_file = false; continue;}
			if (eofDetector == 0) {
                from_file = false;
                // Close batch file as we finished reading from it.
                close_commands_batch_file();
                continue;
			} else {
                // Remove new line from the end of input.
                if (input[strlen(input) - 1] == '\n') {
                    input[strlen(input) - 1] = '\0';
                }
                // print read command.
                printf("Shell > %s\n", input);
			}
		}
		else {
            // Print Shell head.
            printf("Shell > ");
			//read next instruction from console
            eofDetector = read_line(&input, &input_size, stdin);
            if (eofDetector == 0) {
                // ctrl + D is entered.
                break;
            }
		}
		// Remove new line from the end of input.
		if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
		}
		if (strlen(input) > MAX_COMMAND_LEN) {
            fprintf(stderr, "ERROR : input above specified size.\n");
		} else if (strlen(input) > 0) {
            // parse your command here
            int nulled_index = parse_command(input, args, commandtype, background);
            // The nulled_index also represents the number of arguments.
            if (nulled_index > 0) {
                // Tell history to add command.
                add_to_history(input);
                // execute your command here and update if we should continue the loop.
                working = execute_command(( char * const*)args, commandtype, background);
            } else if (nulled_index < 0) {
                // Quotes error.
                fprintf(stderr, "Invalid use of quotes.. please enter a valid command\n");
            }
            // reallocating the nulled index element in the args list.
            if (nulled_index > -1) {
                args[nulled_index] = malloc(sizeof(char) * STRING_MAX_SIZE);
            }
        }
	}
	printf("\n");
	// Free taken resources.
	i = 0;
	for (i = 0; i < MAX_COMMAND_LEN; i++) {
        if (args[i] != 0) {
            free(args[i]);
        }
    }
    free(background);
    free(commandtype);
    free(args);
    free(input);
}
