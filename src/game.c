#include "terminal.h"
#include <bits/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define TARGET_FPS 60.0
#define TARGET_DT (1.0 / TARGET_FPS)
#define ROWS 43
#define COLS 80

static char board[ROWS][COLS];

static double get_time_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec + ts.tv_nsec / 1e9);
}

static void sleep_seconds(double seconds) {
    if (seconds <= 0) return;
    struct timespec ts;
    ts.tv_sec = (time_t)seconds;
    ts.tv_nsec = (long)((seconds - ts.tv_sec) * 1e9);
    nanosleep(&ts, NULL);
}

// static void clear_screen(void) {
//     // \e[1;1H moves the cursor to row 1, column 1
//     // \e[2J clears the entire screen
//     printf("\e[1;1H\e[2J"); 
// }

static void reset_cursor(void) {
    const char buf[] = "\x1B[H";
    terminal_write(buf, sizeof(buf));
}

static void set_borders(char board[ROWS][COLS]) {
    for (int i = 0; i < (COLS - 1) ; i++) {
        board[0][i] = '#';
        board[ROWS-1][i] = '#';
    }
    board[0][COLS-1] = '\n';
    board[ROWS-1][COLS-1] = '\n';

    for (int i = 0; i < ROWS - 1; i++) {
        board[i][0] = '#';
        board[i][COLS-2] = '#';
        board[i][COLS-1] = '\n';
    }
}

static void draw(void) {
    set_borders(board);
    reset_cursor();
    terminal_write(board, ROWS*COLS);
}

int main(void) {
    init_term();
    atexit(restore_term);

    memset(board, ' ', (ROWS * COLS));

    double frame_end, frame_start, dt, frame_time, previous;

    frame_end = get_time_seconds();
    previous = frame_end;

    sig_atomic_t sig = 0;

    while (!(sig = get_quit_status())) {
        frame_start = get_time_seconds();
        dt = frame_start - previous;
        previous = frame_start;

        // TODO:
        // input();
        // update(dt);
        draw();
        // render();

        frame_end = get_time_seconds();
        frame_time = frame_end - frame_start;
        if (frame_time < TARGET_DT) {
            sleep_seconds(TARGET_DT - frame_time);
        }
    }
    return 0;
}
