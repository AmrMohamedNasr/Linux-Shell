#ifndef COMMAND_PARSER_H   /* Include guard */
#define COMMAND_PARSER_H

/*
	- This function should be responsible for importing all details of the command
	- Should specify the type of the command "comment, cd, echo, expression - X=5 -, else"
	- Should specify the arguments of the command
	- Should specify if the command is background or foreground
	- Should consider all parsing special cases, example: many spaces in  "ls     -a"

	- You're left free to decide how to return your imported details of this command

	- Best practice is to use helper function for each collection of logical instructions,
	  example: function for splitting the command by space into array of strings, ..etc
*/
int parse_command( const char* command, char ** args, int * commandType, int * background);

int execute_command( char * const * args, const int * commandType, const int * background);

#endif // COMMAND_PARSER_H
