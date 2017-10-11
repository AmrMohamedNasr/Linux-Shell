#ifndef COMMANDS_H_   /* Include guard */
#define COMMANDS_H_

/**
	- This function should be responsible for implementing the "history" shell command
**/
void history( char* const* args );

/**
	- This function should be responsible for implementing the "cd" shell command
**/
void cd( char* const* args );

/**
	- This function should be responsible for implementing the "export" shell command
**/
void export_var( char* const* args );
/**
	- This function should be responsible for adding a local variable in the shell command.
	@key
	- The key of the variable to add.
	@value
	- The value corresponding to the key.
	@return
	- 1 if it is added successfully, 0 otherwise.
**/
int add_local_variable( char * key, char * value);
/**
	- This function should be responsible for implementing the "echo" shell command
	- Typically should lookup any variable included in the message
**/
void echo( char* const* args );
/**
    - Checks if a string is a valid variable name.
    @variable
    - The string to check.
    @return
    - 1 if it's valid, 0 otherwise.
**/
int valid_shell_name(const char * variable);
/**
    - Checks if a character is a valid variable name character.
    @c
    - The character to check.
    @start
    - If the character is at the start of the variable name, it is equal 1, otherwise 0.
    @return
    - 1 If it is valid, 0 otherwise.

**/
int valid_shell_char(const char c, int start);
#endif // COMMANDS_H_
