#include "command_parser.h"
#include <stdlib.h>

void split_string(const char * message, char ** args, const char delimiter);

void parse_command( const char* command , char** args)
{
	// you should implement this function
	int i = 0;
	split_string(command, args, ' ');
}

void split_string(const char * message, char ** args, const char delimiter) {
    int index = 0, copy, count = 0;
    while (* (message + index) != '\0') {
        if (* (message + index) == delimiter) {
            while(* (message + index) == delimiter) {
                index++;
            }
        } else if (* (message + index) != '\n') {
            copy = 0;
            while(* (message + index) != delimiter && * (message + index) != '\0' && * (message + index) != '\n') {
                *(*(args + count) + copy) = * (message + index);
                index++;
                copy++;
            }
            *(*(args + count) + copy) = '\0';
            count++;
        } else {
            index++;
        }
    }
    args[count] = 0;
    return args;
}

int count_split_strings(const char * message, const char delimiter) {
    int count = 0;
    int index = 0;
    while (* (message + index) != '\0') {
        if (* (message + index) == delimiter) {
            while(* (message + index) == delimiter) {
                index++;
            }
        } else if (* (message + index) != '\n') {
            count++;
            while(* (message + index) != delimiter && * (message + index) != '\0' && * (message + index) != '\n') {
                index++;
            }
        } else {
            index++;
        }
    }
    return count;
}
