#ifndef VARIABLES_H_   /* Include guard */
#define VARIABLES_H_

/**
    - This function should initialize the internal variable table.
    - It will be responsible of making the variable table ready for any subsequent operations.
    - Only needed to be called once.
    - A constructor of sorts.
**/
void initialize_variable_table( void );
/**
	- This function should be responsible for getting the value of a variable
    - First, looks inside the variable table, if hit then return its value.
    - If not, ask the system for the variable, if hit , save it in the table and return it.
    - Otherwise, return null.
    @key
    - The key to search for its value.
    @output_string
    - The string that will contain the value paired with the key if it is found, otherwise it will be set to "".
    @return value
    - If found, it will return the address of output_string, otherwise it will return null.

**/
const char* lookup_variable( const char* key, char * output_string );
/**
	- This function should be responsible for setting the value of a variable
	- If key already exists, update value.
	- If key doesn't exist, add the pair of key, value to the variables table.
	@key
	- The key to update/add.
	@value
	- The value to add.
**/
void set_variable( const char* key , const char* value );
/**
	- This is a helper function to print all your variables
	- Helped much in the debugging or testing
**/
void print_all_variables( void );
/**
    - Frees the resources taken by the variables table.
    - Do it after you finished using the variables table.
    - Will remove all data in the variables table.
**/
void clear_variable_table( void );
#endif // VARIABLES_H_
