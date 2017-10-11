#include "../include/history.h"
#include <string.h>
#include "../include/shell_constants.h"
#include "../include/file_processing.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/string_operations.h"
#include "../include/file_processing.h"

// The history table that will contain all the users history for fast access.
static char ** history_table;
// The real current history table size.
static unsigned long cur_history_size;
// The maximum possible history table size.
static unsigned long max_history_size;

/**
    - Add to the history table a command.
    - Will only add it to the table, isn't responsible of saving it in a file.
    @command
    - The command to add to the table.
**/
void add_to_history_table(const char * command);
/**
    - Read all the history from the file and store it in the table.
**/
void read_all_history(void);

// Implementation (Documentation in headers).
void intialize_history_table(void) {
    // Open history file.
    open_history_file();
    // Allocate memory to the history table.
    cur_history_size = 0;
    max_history_size = HISTORY_TABLE_SIZE;
    history_table = malloc(max_history_size * sizeof(char *));
    int i = 0;
    for (i = 0; i < max_history_size; i++) {
        history_table[i] = malloc(STRING_MAX_SIZE * sizeof(char));
    }
    // Read all history from the file, and store it to the table.
    read_all_history();
}
// Implementation (Documentation in headers).
void clear_history_table(void) {
    // Free resources taken by the history table.
    int i = 0;
    for (i = 0; i < max_history_size; i++) {
        free(history_table[i]);
    }
    free(history_table);
    // Close history file.
    close_history_file();
}
// Implementation (Documentation in headers).
void add_to_history(char * command) {
    // Trim the command to add to the history.
    char * lead_trim_command = malloc((MAX_COMMAND_LEN + 1) * sizeof(char));
    remove_leading_spaces(command, lead_trim_command);
    // Go to the end of file.
    fseek(get_history_file(), 0, SEEK_END);
    if (cur_history_size == 0) {
        // If no previous history, just add the command to the file..
        if (fputs(lead_trim_command, get_history_file()) == EOF) {
            perror("Error while writing to history file ");
        } else {
            // And to the history table too.
            add_to_history_table(lead_trim_command);
            fputc('\n', get_history_file());
        }
        // Flush to print history immediatly.
        fflush(get_history_file());
    } else {
        // Else, check if it is different from the last command in history.
        if (strcmp(history_table[cur_history_size - 1], lead_trim_command) != 0) {
            // If its different, add it to the history.
            if (fputs(lead_trim_command, get_history_file()) == EOF) {
                perror("Error while writing to history file ");
            } else {
                add_to_history_table(lead_trim_command);
                fputc('\n', get_history_file());
            }
            fflush(get_history_file());
        }
    }
    free(lead_trim_command);
}
// Implementation (Documentation above prototype).
void read_all_history(void) {
    char * temp = malloc(STRING_MAX_SIZE * sizeof(char));
    // Go the start of the history file.
    fseek(get_history_file(), 0, SEEK_SET);
    // Loop untill you reach the end of file.
    while (fgets(temp, STRING_MAX_SIZE, get_history_file())) {
        // Add commands to the table after removing the \n sepeartors used in the file.
        if (temp[strlen(temp) - 1] == '\n') {
            temp[strlen(temp) - 1] = '\0';
        }
        add_to_history_table(temp);
    }
    free(temp);
}
// Implementation (Documentation above prototype).
void add_to_history_table(const char * command) {
    // If table is full, double its size.
    if (cur_history_size == max_history_size) {
        max_history_size = max_history_size * 2;
        history_table = realloc(history_table, max_history_size * sizeof(char *));
        int j = cur_history_size;
        for (j = cur_history_size; j < max_history_size; j++) {
            history_table[j] = malloc(STRING_MAX_SIZE * sizeof(char));
            history_table[j][0] = '\0';
        }
    }
    // Copy value into table.
    strcpy(history_table[cur_history_size], command);
    // Increase current table size.
    cur_history_size++;
}
// Implementation (Documentation in headers).
void print_history() {
    unsigned long i = 0;
    for (i = 0; i < cur_history_size; i++) {
        printf("%lu\t%s\n", i + 1, history_table[i]);
    }
}
