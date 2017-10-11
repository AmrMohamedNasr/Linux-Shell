#ifndef SIGNAL_HANDLER_H_INCLUDED
#define SIGNAL_HANDLER_H_INCLUDED
/**
    - Starts the signal handling service.
    - Will assign a handling routine to the sigchld event which fires
    every time a child process ends.
**/
void start_signal_handlers(void);
/**
    - Stops the signal handling service.
**/
void stop_signal_handlers(void);

#endif // SIGNAL_HANDLER_H_INCLUDED
