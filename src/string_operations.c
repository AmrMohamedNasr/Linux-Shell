#include "../include/string_operations.h"
#include <string.h>
#include "../include/shell_constants.h"
#include <stdlib.h>

// Implementation (documentation in headers).
int split_string(const char * message, char ** args, const char * delimiter) {
    char * token;
    char copy[STRING_MAX_SIZE];
    strcpy(copy, message);
    token = strtok(copy, delimiter);
    int i = 0;
    while (token != NULL) {
        strcpy(args[i], token);
        token = strtok(NULL, delimiter);
        i++;
    }
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
    if (end < 0 || end > STRING_MAX_SIZE) {
        return;
    }
    char temp[STRING_MAX_SIZE];
    strcpy(temp, source);
    temp[end] = '\0';
    strcat(destination, temp + start);
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
// Implementation (documentation in headers).
char * read_line(char ** output, int * size, FILE * source) {
    int i = 0;
    char c = fgetc(source);
    if (c == EOF) {
        return NULL;
    }
    while (c != EOF && c != '\n') {
        (*output)[i] = c;
        c = fgetc(source);
        i++;
        if (i == *size) {
            *size *= 2;
            *output = realloc(*output, *size * sizeof(char));
        }
    }
    if (c != EOF) {
        (*output)[i] = c;
        i++;
        if (i == *size) {
            *size *= 2;
            *output = realloc(*output, *size * sizeof(char));
        }
    }
    (*output)[i] = '\0';
    return *output;
}
