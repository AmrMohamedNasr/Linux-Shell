#ifndef STRING_OPERATIONS_H_INCLUDED
#define STRING_OPERATIONS_H_INCLUDED
#include <stdio.h>

/**
    - splits the string message around any char in the string of delimiters and
    stores the results in args.
    @message
    - The string to split, will not be affected by the split.
    @args
    - The array of strings to save the result in.
    @delimiter
    - The characters to split around.
    @return
    - The numbers of substrings resulting of splitting.
**/
int split_string(const char * message, char ** args, const char * delimiter);
/**
    - Removes all leading white space characters in front of a string.
    @message
    - The string to remove the leading white space characters on, will not be affected by the operation.
    @output
    - The string that will store the result of the split.
**/
void remove_leading_spaces(const char * message, char * output);
/**
    - Reverses the characters of a string.
    @string
    - The string to be reserved, result is inplace meaning the string will be changed.
    - Save a copy if you want to have backup to it.
**/
void reverse_string(char * string);
/**
    - Concate two strings while taking a certain number of characters from the source string.
    @source
    - The string to be added to the other, will not be changed by the operation.
    @start
    - The starting character index to copy from the source. INCLUSIVE.
    @end
    - The ending character index. EXCLUSIVE.
    @destination
    - The string that will contain the result of the operation.
**/
void cat_string(int start, int end, char * destination, const char * source);
/**
    - Checks if character is a whitespace character.
    @c
    - The character to test.
    @return
    - 1 if it is a white space character, 0 if it is not.
**/
int isSpace(const char c);
/**
    - Reads a line from a stream. Line ends when EOF or newline are encounted.
    @output
    - Pointer to the string pointer that input will be read into, might get resized during the read operation.
    @size
    - The current size of the string memory.
    @source
    - The stream source to read from.
    @return
    - NULL if EOF is encountered, else return the string pointer.
**/
char * read_line(char ** ouput, int * size, FILE * source);
#endif // STRING_OPERATIONS_H_INCLUDED
