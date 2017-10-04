#include "variables.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell_constants.h"

static char *** variablesTable;
static int size = 0;
static int currentMaxSize = VAR_TABLE_SIZE;

void double_table_size(void);
int get_key_index(const char * key);

void initialize_variable_table(void) {
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

const char* lookup_variable( const char* key, char * output_string )
{
	int keyIndex = get_key_index(key);
	strcpy(output_string, "");
	if (keyIndex == -1) {
        char * result = getenv(key);
        if (result != NULL) {
            strcpy(output_string, getenv(key));
            set_variable(key, output_string);
        } else {
            output_string = 0;
        }
	} else {
        strcpy(output_string, variablesTable[keyIndex][1]);
	}
	return output_string;
}

int get_key_index(const char * key) {
    int result = -1;
    int i = 0;
    for (i = 0; i < size; i++) {
        if (strcmp(key, variablesTable[i][0]) == 0) {
            result = i;
            break;
        }
    }
    return result;
}

void set_variable( const char* key , const char* value )
{
	if (size >= currentMaxSize) {
        double_table_size();
	}
	int keyIndex = get_key_index(key);
	if (keyIndex == -1) {
        strcpy(variablesTable[size][0], key);
        strcpy(variablesTable[size][1], value);
        size++;
    } else {
        strcpy(variablesTable[keyIndex][1], value);
    }
}

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

void print_all_variables( void )
{
	int i = 0;
	for (i = 0; i < size; i++) {
        printf("%s=%s\n", variablesTable[i][0], variablesTable[i][1]);
	}
}

void clear_variable_table() {
    free(variablesTable);
}
