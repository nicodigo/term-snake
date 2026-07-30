#ifndef TERMINAL_H
#define TERMINAL_H

#include <signal.h>
int init_term(void);
void restore_term(void);
sig_atomic_t get_quit_status(void);

#endif
