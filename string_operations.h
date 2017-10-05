#ifndef STRING_OPERATIONS_H_INCLUDED
#define STRING_OPERATIONS_H_INCLUDED

int split_string(const char * message, char ** args, const char * delimiter);

void remove_leading_spaces(const char * message, char * output);

void reverse_string(char * string);

#endif // STRING_OPERATIONS_H_INCLUDED
