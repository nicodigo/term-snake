#include "snake.h"
#include "terminal.h"
#include <bits/time.h>
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#define TARGET_FPS 60.0
#define TARGET_DT (1.0 / TARGET_FPS)
#define ROWS 43
#define COLS 80

struct food {
    int x, y;
};

static char board[ROWS][COLS];
static struct snake my_snake;
static struct food my_food = {.x = 0, .y = 0};
static sig_atomic_t quit = 0;
static int game_over = 0;
static struct snake_node grow_here;
static double snake_tick = 0.150;

static double get_time_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec + ts.tv_nsec / 1e9);
}

static void sleep_seconds(double seconds) {
    if (seconds <= 0)
        return;
    struct timespec ts;
    ts.tv_sec = (time_t)seconds;
    ts.tv_nsec = (long)((seconds - ts.tv_sec) * 1e9);
    nanosleep(&ts, NULL);
}

static void reset_cursor(void) {
    const char buf[] = "\x1B[H";
    terminal_write(buf, sizeof(buf));
}

static void set_borders(char board[ROWS][COLS]) {
    for (int i = 0; i < (COLS - 1); i++) {
        board[0][i] = '#';
        board[ROWS - 1][i] = '#';
    }
    board[0][COLS - 1] = '\n';
    board[ROWS - 1][COLS - 1] = '\n';

    for (int i = 0; i < ROWS - 1; i++) {
        board[i][0] = '#';
        board[i][COLS - 2] = '#';
        board[i][COLS - 1] = '\n';
    }
}

static void set_snake(char board[ROWS][COLS]) {
    struct snake_node *node = my_snake.head;

    if (node != NULL) {
        board[node->y][node->x] = '@';
        node = node->next_node;
    }

    while (node != NULL) {
        board[node->y][node->x] = 'O';
        node = node->next_node;
    }
}

static void set_food(void) {
    if ((my_food.x != my_food.y) != 0) {
        board[my_food.y][my_food.x] = '*';
    }
}

static void draw(void) {
    memset(board, ' ', ROWS * COLS);
    set_borders(board);
    set_snake(board);
    set_food();
    reset_cursor();
    terminal_write(board, ROWS * COLS);
}

static void move_snake(void) {
    struct snake_node prev_node, aux_node;
    struct snake_node *node_ptr;

    grow_here = *my_snake.tail;

    prev_node = *my_snake.head;
    node_ptr = my_snake.head->next_node;
    while (node_ptr != NULL) {
        aux_node = *node_ptr;

        node_ptr->x = prev_node.x;
        node_ptr->y = prev_node.y;

        prev_node = aux_node;
        node_ptr = node_ptr->next_node;
    }

    switch (my_snake.direction) {
    case 'u': {
        my_snake.head->y--;
        break;
    }
    case 'r': {
        my_snake.head->x++;
        break;
    }
    case 'd': {
        my_snake.head->y++;
        break;
    }
    case 'l': {
        my_snake.head->x--;
        break;
    }
    default: {
        return;
    }
    }
}

static int snake_hit_wall(void) {
    int collision = 0;

    if (my_snake.head->x <= 0) {
        collision = 1;
    } else if (my_snake.head->y <= 0) {
        collision = 1;
    } else if (my_snake.head->x >= (COLS - 1)) {
        collision = 1;
    } else if (my_snake.head->y >= (ROWS - 1)) {
        collision = 1;
    }

    return collision;
}

static int snake_hit_itself(void) {
    struct snake_node head = *my_snake.head;
    struct snake_node *node = my_snake.head->next_node;

    while (node != NULL) {
        if (node->x == head.x && node->y == head.y) {
            return 1;
        }
        node = node->next_node;
    }
    return 0;
}

static void spawn_food(void) {
    int x, y;

    do {
        x = rand() % COLS;
        y = rand() % ROWS;
    } while (is_snake(&my_snake, x, y));

    my_food.x = x;
    my_food.y = y;
}

static int snake_ate_food(void) {
    return (my_snake.head->x == my_food.x && my_snake.head->y == my_food.y);
}


static void update(void) {
    move_snake();
    if (snake_hit_wall()) {
        game_over = 1;
        return;
    }

    if (snake_hit_itself()) {
        game_over = 1;
        return;
    }

    if (snake_ate_food()) {
        grow_snake(&my_snake, grow_here.x, grow_here.y);
        spawn_food();
        snake_tick -= 0.005;
    }
}

static int is_opposite(char new_direction) {
    switch (my_snake.direction) {
    case 'u':
        return new_direction == 'd';
    case 'r':
        return new_direction == 'l';
    case 'd':
        return new_direction == 'u';
    case 'l':
        return new_direction == 'r';
    default:
        return 0;
    }
}

static void input(void) {
    char key, direction;
    terminal_read(&key, sizeof(key));
    key = tolower(key);

    switch (key) {
    case 'k': {
        direction = 'u';
        break;
    }
    case 'l': {
        direction = 'r';
        break;
    }
    case 'j': {
        direction = 'd';
        break;
    }
    case 'h': {
        direction = 'l';
        break;
    }
    default: {
        return;
    }
    }

    if (direction != my_snake.direction && !is_opposite(direction)) {
        my_snake.direction = direction;
    }
}

int main(void) {
    init_term();
    atexit(restore_term);
    my_snake = new_snake(COLS / 2, ROWS / 2, 'r');
    grow_snake(&my_snake, my_snake.head->x - 1, my_snake.head->y);
    grow_snake(&my_snake, my_snake.head->x - 2, my_snake.head->y);
    grow_here = *my_snake.tail;
    memset(board, ' ', (ROWS * COLS));
    srand((unsigned)time(NULL));
    spawn_food();

    double frame_end, frame_start, dt, frame_time, previous;

    frame_end = get_time_seconds();
    previous = frame_end;

    double accumulator = 0.0;

    while (!(quit = get_quit_status())) {
        frame_start = get_time_seconds();
        dt = frame_start - previous;
        previous = frame_start;

        accumulator += dt;
        while (accumulator >= snake_tick) {
            input();
            update();
            accumulator -= snake_tick;
        }

        if (game_over) {
            break;
        }

        draw();

        frame_end = get_time_seconds();
        frame_time = frame_end - frame_start;
        if (frame_time < TARGET_DT) {
            sleep_seconds(TARGET_DT - frame_time);
        }
    }
    return 0;
}
