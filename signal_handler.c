#include "signal_handler.h"
#include <stdio.h>
#include <signal.h>
#include "file_processing.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
/**
    - Our handler routine that will be responsible about:
        1- Removing zombie processes.
        2- Printing message to log file.
    @signo
    - Is a signal variable send with the signal.
**/
void catch_sigchld(int signo);
// Implementation (Documentation in headers).
void start_signal_handlers() {
    // open log file.
    open_log_file();
    // Attach our routine to the event.
    if (signal(SIGCHLD, catch_sigchld) == SIG_ERR) {
        fprintf(stderr, "An error occurred while setting sigchld signal handler.\n");
    }
}
// Implementation (Documentation above prototype).
void catch_sigchld(int signo) {
    // Remove any zombie process.
    waitpid(-1, NULL, WNOHANG);
    // Print message to file.
    fprintf(get_log_file(), "Child process was terminated\n");
    // Flush to immediatly print.
    fflush(get_log_file());
}
// Implementation (Documentation in headers).
void stop_signal_handlers() {
    // close log file.
    close_log_file();
}
