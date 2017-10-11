#ifndef HISTORY_H_INCLUDED
#define HISTORY_H_INCLUDED
/**
    - Add a command to the history.
    - Will only add if its different from the previous command even with a space.
    - Leading spaces will be trimmed.
**/
void add_to_history(char * command);
/**
    - Will close the history table and free any resources taken.
**/
void clear_history_table(void);
/**
    - Initializes the history table taking the resources it needs.
**/
void intialize_history_table(void);
/**
    - Print all history of the user.
**/
void print_history(void);

#endif // HISTORY_H_INCLUDED
