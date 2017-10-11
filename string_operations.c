#include "string_operations.h"
#include <string.h>
#include "shell_constants.h"
#include <stdlib.h>

// Implementation (documentation in headers).
int split_string(const char * message, char ** args, const char * delimiter) {
    char * token;
    char * copy = strdup(message);
    token = strtok(copy, delimiter);
    int i = 0;
    while (token != NULL) {
        strcpy(args[i], token);
        token = strtok(NULL, delimiter);
        i++;
    }
    free(copy);
    return i;
}
// Implementation (documentation in headers).
void remove_leading_spaces(const char * message, char * output) {
    int i = 0;
    while (message[i] != '\0' && isSpace(message[i]) == 1) {
        i++;
    }
    strcpy(output, message + i);
}
// Implementation (documentation in headers).
void cat_string(int start, int end, char * destination, const char * source) {
    char * temp = strdup(source);
    temp[end] = '\0';
    strcat(destination, temp + start);
    free(temp);
}
// Implementation (documentation in headers).
void reverse_string(char * string) {
    int length = strlen(string);
    int i = 0;
    int end_number = length / 2;
    while (i < end_number) {
        string[i] ^= string[length - i - 1];
        string[length - i - 1] ^= string[i];
        string[i] ^= string[length - i - 1];
        i++;
    }
}
// Implementation (documentation in headers).
int isSpace(const char c) {
    return (c == '\n' || c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\v') ? 1 : 0;
}
