#include "variables.h"
#include <stdlib.h>

const char* lookup_variable( const char* key )
{
	// you should implement this function
	return getenv(key);
}

void set_variable( const char* key , const char* value )
{
	// you should implement this function
}

void print_all_variables( void )
{
	// you should implement this function
}
