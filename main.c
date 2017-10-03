#include "variables.h"
#include "commands.h"
#include "command_parser.h"
#include "environment.h"
#include "file_processing.h"
#include <stdlib.h>

#define     MAX_COMMAND_LEN         512
typedef enum{ false = 0 , true = 1 } bool ;

void start_shell(bool read_from_file);
void shell_loop(bool input_from_file);

int main(int argc, char *argv[], char** envp)
{
    setup_environment();

    // any other early configuration should be here
    if(argc > 2) {
        fprintf(stderr, "Invalid number of arguments. Command should be entered as <shell [batch file]>\n");
    } else if( argc == 2 ){
        start_shell(true);
    }
    else{
        start_shell(false);
    }

    return 0;
}

void start_shell(bool read_from_file)
{
	cd(""); // let shell starts from home

	if(read_from_file){
		// file processing functions should be called from here

		shell_loop(true);
	}
	else{
		shell_loop(false);
	}
}

void shell_loop(bool input_from_file)
{
	bool from_file = input_from_file;
    char * input = malloc(513 * sizeof(char));
	while(true){
		printf("Shell>");
		if(from_file){
			//read next instruction from file

			// if end of file {from_file = false; continue;}
		}
		else{
			//read next instruction from console
            fgets(input, 513, stdin);
		}

		//parse your command here
		char **args = malloc (sizeof (char *) * (count_split_strings(input, ' ') + 1));
		int i = 0;
		for (i = 0; i < count_split_strings(input, ' '); i++) {
            args[i] = malloc(sizeof(char) * MAX_COMMAND_LEN);
		}
        parse_command(input, args);
        int pid = fork();
        if (pid < 0) {

        } else if (pid == 0) {
            execvp(args[0], args);
        } else {
            waitpid(pid);
        }
		for (i = 0; i < count_split_strings(input, ' ') - 1; i++) {
            free(args[i]);
		}
		free(args);
		//execute your command here

		/*
			you don't need to write all logic here, a better practice is to call functions,
			each one contains a coherent set of logical instructions
		*/
	}
}
