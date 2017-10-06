#include "signal_handler.h"
#include <stdio.h>
#include <signal.h>
#include "file_processing.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void catch_sigchld(int signo);

void start_signal_handlers() {
    open_log_file();
    if (signal(SIGCHLD, catch_sigchld) == SIG_ERR) {
        fprintf(stderr, "An error occurred while setting sigchld signal handler.\n");
    }
}

void catch_sigchld(int signo) {
    waitpid(-1, NULL, WNOHANG);
    fprintf(get_log_file(), "Child process was terminated\n");
    fflush(get_log_file());
}

void stop_signal_handlers() {
    close_log_file();
}
