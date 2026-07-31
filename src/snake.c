/* Definition of the representation and behaviour of the snake*/

#include "snake.h"
#include <stddef.h>
#include <stdlib.h>

struct snake new_snake(int x, int y, char direction) {
    size_t len = 0;
    struct snake_node *new_node =
        (struct snake_node *)malloc(sizeof(struct snake_node));

    if (new_node != NULL) {
        new_node->next_node = NULL;
        new_node->x = x;
        new_node->y = y;
        len = 1;
    }

    struct snake new_snake = {
        .len = len,
        .head = new_node,
        .tail = new_node,
        .direction = direction,
    };
    return new_snake;
}

void destroy_snake(struct snake s) {
    struct snake_node *node = s.head;
    struct snake_node *next_node = NULL;
    while (node != NULL) {
        next_node = node->next_node;
        free(node);
        node = next_node;
    }
}

/* Grows snake by 1 node, new node has (x,y) coordinates*/
void grow_snake(struct snake *s, int x, int y) {
    if (s == NULL) {
        return;
    }
    if (s->tail == NULL) {
        return;
    }

    struct snake_node *new_node =
        (struct snake_node *)malloc(sizeof(struct snake_node));

    if (new_node == NULL) {
        return;
    }
    new_node->x = x;
    new_node->y = y;

    s->tail->next_node = new_node;
    s->tail = new_node;
    s->len++;
}

int is_snake(struct snake *s, int x, int y) {
    struct snake_node *node = s->head;
    int found = 0;

    while (node != NULL) {
        if (node->x == x && node->y == y) {
            found = 1;
            break;
        }
        node = node->next_node;
    }

    return found;
}
