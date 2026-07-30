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
#define ROWS 32
#define COLS 128

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
    printf("\x1B[H");
}

static void draw_board(char board[ROWS][COLS]) {
    reset_cursor();
    for (int i = 0; i < COLS + 2; i++) {
        printf("-");
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("|");
        for (int j = 0; j < COLS; j++) {
            printf("%c", board[i][j]);
        }
        printf("|\n");
    }

    for (int i = 0; i < COLS + 2; i++) {
        printf("-");
    }
    printf("\n");
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
        draw_board(board);
        // render();

        frame_end = get_time_seconds();
        frame_time = frame_end - frame_start;
        if (frame_time < TARGET_DT) {
            sleep_seconds(TARGET_DT - frame_time);
        }
    }
    return 0;
}
