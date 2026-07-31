/* Terminal init and restore code
 * For now this is the file that sets the terminal behavior
 * Such as the configuration of termios for non canonical mode
 * Additionally handles signals in here, im not sure of this but for now
 * I will do it here, so...
 * TODO: to move it or not to move it, thats the question*/
#include "terminal.h"
#include <signal.h>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>

struct termios t_orig, t_new;
volatile sig_atomic_t quit = 0;

static void signal_handler(int sig) { quit = sig; }

int init_term(void) {
    int status = 0;

    // configurar sigaction
    // TODO: -- explore further on sigaction. For now i want to center on the
    // game.
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if ((status = sigaction(SIGINT, &sa, NULL)) != 0) {
        return status;
    }

    if ((status = sigaction(SIGTERM, &sa, NULL)) != 0) {
        return status;
    }

    if ((status = sigaction(SIGHUP, &sa, NULL)) != 0) {
        return status;
    }

    // termios config
    // NOTE: -- ICANON makes individual keypresses readables, without waiting
    // for NL. ECHO dictates if the input characters are shown on screen. explore
    // other flags in detail later.
    if ((status = tcgetattr(STDIN_FILENO, &t_orig)) != 0) {
        return status;
    }

    t_new = t_orig;

    t_new.c_lflag &= ~(ICANON | ECHO);
    t_new.c_cc[VMIN] = 0;
    t_new.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    return status;
}

void restore_term(void) { tcsetattr(STDIN_FILENO, TCSAFLUSH, &t_orig); }

sig_atomic_t get_quit_status(void) { return quit; }

ssize_t terminal_write(const void *buf, size_t n) {
    return write(STDOUT_FILENO, buf, n);
}

ssize_t terminal_read(void *buf, size_t n) {
    return read(STDIN_FILENO, buf, n);
}
