#ifndef COMMAND_PARSER_H   /* Include guard */
#define COMMAND_PARSER_H

/**
	- This function should be responsible for importing all details of the command
	- Should specify the type of the command "comment, cd, echo, expression - X=5 -, else"
	- Should specify the arguments of the command
	- Should specify if the command is background or foreground
	- Should consider all parsing special cases, example: many spaces in  "ls     -a"
	@command
	- The command to parse.
	@background
	- Returns in it if it is a background operation.
	@args
	- Return the list of arguments parsed from the command.
	@commandType
	- Returns the command type of the entered command.
	@return
	- The number of arguments.
**/
int parse_command( const char* command, char ** args, int * commandType, int * background);
/**
    - This function executs the command by taking the arguments, command type and background variable.
    @return
    - 1 If the shell should continue, 0 otherwise.
**/
int execute_command( char * const * args, const int * commandType, const int * background);

#endif // COMMAND_PARSER_H
