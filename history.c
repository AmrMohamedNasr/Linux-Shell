#include "history.h"
#include <string.h>
#include "shell_constants.h"
#include "file_processing.h"
#include <stdio.h>
#include <stdlib.h>
#include "string_operations.h"
#include "file_processing.h"

static char ** history_table;
static unsigned long cur_history_size;
static unsigned long max_history_size;

void add_to_history_table(const char * command);
void read_all_history(void);

void intialize_history_table(void) {
    open_history_file();
    cur_history_size = 0;
    max_history_size = HISTORY_TABLE_SIZE;
    history_table = malloc(max_history_size * sizeof(char *));
    int i = 0;
    for (i = 0; i < max_history_size; i++) {
        history_table[i] = malloc(STRING_MAX_SIZE * sizeof(char));
    }
    read_all_history();
}

void clear_history_table(void) {
    int i = 0;
    for (i = 0; i < max_history_size; i++) {
        free(history_table[i]);
    }
    free(history_table);
    close_history_file();
}

void add_to_history(char * command) {
    char * lead_trim_command = malloc(MAX_COMMAND_LEN * sizeof(char));
    remove_leading_spaces(command, lead_trim_command);
    fseek(get_history_file(), 0, SEEK_END);
    if (cur_history_size == 0) {
        if (fputs(lead_trim_command, get_history_file()) == EOF) {
            perror("Error while writing to history file ");
        } else {
            add_to_history_table(lead_trim_command);
            fputc('\n', get_history_file());
        }
        fflush(get_history_file());
    } else {
        if (strcmp(history_table[cur_history_size - 1], lead_trim_command) != 0) {
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

void read_all_history(void) {
    char * temp = malloc(MAX_COMMAND_LEN * sizeof(char));
    while (fgets(temp, MAX_COMMAND_LEN, get_history_file())) {
        if (temp[strlen(temp) - 1] == '\n') {
            temp[strlen(temp) - 1] = '\0';
        }
        add_to_history_table(temp);
    }
    free(temp);
}

void add_to_history_table(const char * command) {
    if (cur_history_size == max_history_size) {
        max_history_size = max_history_size * 2;
        history_table = realloc(history_table, max_history_size * sizeof(char *));
        int j = cur_history_size;
        for (j = cur_history_size; j < max_history_size; j++) {
            history_table[j] = malloc(STRING_MAX_SIZE * sizeof(char));
            history_table[j][0] = '\0';
        }
    }
    strcpy(history_table[cur_history_size], command);
    cur_history_size++;
}

void print_history() {
    unsigned long i = 0;
    for (i = 0; i < cur_history_size; i++) {
        printf("%lu\t%s\n", i + 1, history_table[i]);
    }
}
