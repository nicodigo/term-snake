#ifndef SNAKE_H
#define SNAKE_H
#include <stddef.h>
struct snake_node {
    int x, y;
    struct snake_node *next_node;
};


struct snake {
    size_t len;
    struct snake_node *head;
    struct snake_node *tail;
    unsigned char direction;
};

struct snake new_snake(int x, int y, char direction);
void destroy_snake(struct snake s);
void grow_snake(struct snake *s, int x, int y);
int is_snake(struct snake *s, int x, int y);
#endif
