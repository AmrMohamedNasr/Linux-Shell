#include "file_processing.h"
#include "variables.h"
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define         FILE_NUMBER     3
#define         HISTORY_INDEX   0
#define         LOGS_INDEX      1
#define         BATCH_INDEX     2

static FILE * files [FILE_NUMBER];
/*
	history file section
*/
void open_history_file()
{
	char * path = malloc(PATH_MAX * sizeof(char));
	lookup_variable("HLD", path);
    strcat(path, "/history.log");
    files[HISTORY_INDEX] = fopen(path, "a+");
    if (files[HISTORY_INDEX] == NULL) {
        perror("Error opening history file ");
    }
}

FILE* get_history_file()
{
	return files[HISTORY_INDEX];
}

void close_history_file()
{
	if (fclose(files[HISTORY_INDEX]) != 0) {
        perror("History file closing error ");
	}
	files[HISTORY_INDEX] = NULL;
}


/*
	log file section
*/
void open_log_file()
{
	char * path = malloc(PATH_MAX * sizeof(char));
	lookup_variable("HLD", path);
    strcat(path, "/logs.log");
    files[LOGS_INDEX] = fopen(path, "w");
    if (files[LOGS_INDEX] == NULL) {
        perror("Error opening logs file ");
    }
}

FILE* get_log_file()
{
	return files[LOGS_INDEX];
}

void close_log_file()
{
	if (fclose(files[LOGS_INDEX]) != 0) {
        perror("Logs file closing error ");
	}
	files[LOGS_INDEX] = NULL;
}


/*
	CommandsBatch file section
*/
void open_commands_batch_file(const char * file_dir)
{
	char * path = malloc(PATH_MAX * sizeof(char));
	char cwd [PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
       perror("Getting working directory error ! ");
    }
	if (file_dir[0] == '.' && file_dir[1] == '/') {
        strcpy(path, cwd);
        strcat(path, file_dir+1);
	} else if (file_dir[0] != '/') {
        strcpy(path, cwd);
        strcat(path, "/");
        strcat(path, file_dir);
	} else {
        strcpy(path, file_dir);
	}
	printf("%s\n", path);
	// + to keep sure it is not a directory.
	files[BATCH_INDEX] = fopen(path, "r+");
    if (files[BATCH_INDEX] == NULL) {
        perror("Error opening batch file ");
    }
}

FILE* get_commands_batch_file()
{
	return files[BATCH_INDEX];
}

void close_commands_batch_file()
{
	if (fclose(files[BATCH_INDEX]) != 0) {
        perror("Batch file closing error ");
	}
	files[BATCH_INDEX] = NULL;
}
