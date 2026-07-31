#ifndef TERMINAL_H
#define TERMINAL_H

#include <signal.h>
#include <stdio.h>
int init_term(void);
void restore_term(void);
sig_atomic_t get_quit_status(void);
ssize_t terminal_write(const void *buf, size_t n);
ssize_t terminal_read(void *buf, size_t n);

#endif
