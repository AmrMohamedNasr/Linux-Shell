#include "../include/file_processing.h"
#include "../include/variables.h"
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// The number of files.
#define         FILE_NUMBER     3
// The history file index.
#define         HISTORY_INDEX   0
// The logs file index.
#define         LOGS_INDEX      1
// The batch file index.
#define         BATCH_INDEX     2

// Array containing the file pointers.
static FILE * files [FILE_NUMBER];

/*
	history file section
*/
// Implementation (Documented in headers).
void open_history_file()
{
	char * path = malloc(PATH_MAX * sizeof(char));
	// Getting the path for the file.
	lookup_variable("HLD", path);
	strcat(path, "/history.log");
    // Open file in append mode.
    files[HISTORY_INDEX] = fopen(path, "a+");
    if (files[HISTORY_INDEX] == NULL) {
        perror("Error opening history file ");
    }
    free(path);
}
// Implementation (Documented in headers).
FILE* get_history_file()
{
	return files[HISTORY_INDEX];
}
// Implementation (Documented in headers).
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
// Implementation (Documented in headers).
void open_log_file()
{
	char * path = malloc(PATH_MAX * sizeof(char));
	// Get path to logs file.
	lookup_variable("HLD", path);
    strcat(path, "/logs.log");
    // Open in write mode.
    files[LOGS_INDEX] = fopen(path, "w");
    if (files[LOGS_INDEX] == NULL) {
        perror("Error opening logs file ");
    }
    free(path);
}
// Implementation (Documented in headers).
FILE* get_log_file()
{
	return files[LOGS_INDEX];
}
// Implementation (Documented in headers).
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
// Implementation (Documentation in headers).
void open_commands_batch_file(const char * file_dir)
{
	char * path = malloc(PATH_MAX * sizeof(char));
	char cwd [PATH_MAX];
    // Get path of batch file.
    // Get working directory.
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
       perror("Getting working directory error ! ");
    }
    // If relative, add working directory to it.
	if (file_dir[0] == '.' && file_dir[1] == '/') {
        strcpy(path, cwd);
        strcat(path, file_dir+1);
	// If not absolute , add working directory.
	} else if (file_dir[0] != '/') {
        strcpy(path, cwd);
        strcat(path, "/");
        strcat(path, file_dir);
    // Else its absolute path.
	} else {
        strcpy(path, file_dir);
	}
	// + to keep sure it is not a directory.
	files[BATCH_INDEX] = fopen(path, "r+");
    if (files[BATCH_INDEX] == NULL) {
        perror("Error opening batch file ");
    }
    free(path);
}
// Implementation (Documented in headers).
FILE* get_commands_batch_file()
{
	return files[BATCH_INDEX];
}
// Implementation (Documented in headers).
void close_commands_batch_file()
{
	if (fclose(files[BATCH_INDEX]) != 0) {
        perror("Batch file closing error ");
	}
	files[BATCH_INDEX] = NULL;
}
