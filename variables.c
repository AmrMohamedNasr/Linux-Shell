#include "variables.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell_constants.h"

// The table string array containing the pairs
static char *** variablesTable;
// The current real size of the table
static int size;
// The maximum size of the table.
static int currentMaxSize;
/**
    - Doubles the size of the table with deleting the elements.
    - To use when table reaches maximum size.
**/
void double_table_size(void);
/**
    - searches the table for a defined key.
    @key
    - The key to search for.
    @return
    - The index of the key in the table if it is found, otherwise -1.

**/
int get_key_index(const char * key);

// Implementation (documented in header).
void initialize_variable_table(void) {
    size = 0;
    currentMaxSize = VAR_TABLE_SIZE;
    variablesTable = malloc(currentMaxSize * sizeof(char **));
    int i = 0;
    for (i = 0; i < currentMaxSize; i++) {
        variablesTable[i] = malloc(2 * sizeof(char *));
        variablesTable[i][0] = malloc(STRING_MAX_SIZE * sizeof(char));
        variablesTable[i][0][0] = '\0';
        variablesTable[i][1] = malloc(STRING_MAX_SIZE * sizeof(char));
        variablesTable[i][1][0] = '\0';
    }
}
// Implementation (documented in header).
const char* lookup_variable( const char* key, char * output_string )
{
    // search for the index of the key in the table.
	int keyIndex = get_key_index(key);
	// Defaults the output value.
	strcpy(output_string, "");
	if (keyIndex == -1) {
        // If not found in table, search it in the enviroment.
        char * result = getenv(key);
        if (result != NULL) {
            // If found, copy the result to the output string, then add the key,value to the table.
            strcpy(output_string, getenv(key));
            set_variable(key, output_string);
        } else {
            // If not found at all, return null pointer in the return result and an empty string in the output.
            output_string = 0;
        }
	} else {
        // If found get its value.
        strcpy(output_string, variablesTable[keyIndex][1]);
	}
	return output_string;
}
// Implementation (documented above prototype)
int get_key_index(const char * key) {
    int result = -1;
    int i = 0;
    // Loop on valid table, if you get a hit get its index.
    for (i = 0; i < size; i++) {
        if (strcmp(key, variablesTable[i][0]) == 0) {
            result = i;
            break;
        }
    }
    return result;
}
// Implementation (documented in header).
void set_variable( const char* key , const char* value )
{
    // If the size is already at maximum capacity, double the table size.
	if (size >= currentMaxSize) {
        double_table_size();
	}
	// Check If element is already in table.
	int keyIndex = get_key_index(key);
	if (keyIndex == -1) {
        // If not, add a new entry to the table.
        strcpy(variablesTable[size][0], key);
        strcpy(variablesTable[size][1], value);
        size++;
    } else {
        // Otherwise, just update the value.
        strcpy(variablesTable[keyIndex][1], value);
        // If enviromental variable update it in the enviroment too.
        if (getenv(key) != NULL) {
            setenv(key, value, 1);
        }
    }
}
// Implementation (documented in prototype)
void double_table_size(void) {
    currentMaxSize = size * 2;
    variablesTable = realloc(variablesTable, currentMaxSize * sizeof(char **));
    int i = size;
    for (i = size; i < currentMaxSize; i++) {
        variablesTable[i] = malloc(2 * sizeof(char *));
        variablesTable[i][0] = malloc(STRING_MAX_SIZE * sizeof(char));
        variablesTable[i][0][0] = '\0';
        variablesTable[i][1] = malloc(STRING_MAX_SIZE * sizeof(char));
        variablesTable[i][1][0] = '\0';
    }
}
// Implementation (documented in header).
void print_all_variables( void )
{
	int i = 0;
	for (i = 0; i < size; i++) {
        printf("%s=%s\n", variablesTable[i][0], variablesTable[i][1]);
	}
}
// Implementation (documented in header).
void clear_variable_table() {
    int i = 0;
    for (i = 0; i < currentMaxSize; i++) {
        free(variablesTable[i][0]);
        free(variablesTable[i][1]);
        free(variablesTable[i]);
    }
    free(variablesTable);
}
