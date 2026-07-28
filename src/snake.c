#include "terminal.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    init_term();
    atexit(restore_term);
    sig_atomic_t sig = 0;

    while (!(sig = get_quit_status())) {
    }
    printf("Adios mundo... Sig = %d\n", sig);

    return 0;
}
