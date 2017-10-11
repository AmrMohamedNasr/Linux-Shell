#ifndef FILE_PROCESSING_H_   /* Include guard */
#define FILE_PROCESSING_H_
#include <stdio.h>
/*
	history file basic functions' prototypes
*/
/**
    - Open the history file.
**/
void open_history_file();
/**
    - Return the pointer value to the history file.
    @return
    - 0 If file not opened, real value otherwise.
**/
FILE* get_history_file();
/**
    - Close the history file.
**/
void close_history_file();

/*
	log file basic functions' prototypes
*/
/**
    - Open the logs file.
**/
void open_log_file();
/**
    - Return the pointer value to the logs file.
    @return
    - 0 If file not opened, real value otherwise.
**/
FILE* get_log_file();
/**
    - Close the logs file.
**/
void close_log_file();

/*
	CommandsBatch file basic functions' prototypes
*/
/**
    - Open the batch file.
**/
void open_commands_batch_file(const char * file_dir);
/**
    - Return the pointer value to the batch file.
    @return
    - 0 If file not opened, real value otherwise.
**/
FILE* get_commands_batch_file();
/**
    - Close the batch file.
**/
void close_commands_batch_file();


#endif // FILE_PROCESSING_H_
