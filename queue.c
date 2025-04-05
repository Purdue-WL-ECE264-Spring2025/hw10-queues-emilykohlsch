#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>

struct list_node *new_node(struct game_state state);

void enqueue(struct queue *q, struct game_state state) {
  struct list_node *node = new_node(state);
    if (!node) return;

    if (!q->data.head) {
        q->data.head = node;
    } else {
        struct list_node *temp = q->data.head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = node;
    }
}

int is_solved(struct game_state state) {
    int solved[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (state.tiles[row][col] != solved[row][col]) {
                return 0;  // Not solved
            }
        }
    }

    return 1;  // Solved
}

struct game_state dequeue(struct queue *q) { 
   if (!q->data.head) {
        exit(1);  // Handle the case where the queue is empty
    }

    struct list_node *temp = q->data.head;
    struct game_state state = deserialize(temp->value); 
    q->data.head = q->data.head->next;
    free(temp);

    return state;
}

int number_of_moves(struct game_state start) { 
   struct queue q = {0};
    enqueue(&q, start);
    
    while (q.data.head) {
        struct game_state current = dequeue(&q);

        if (is_solved(current)) {
            return current.num_steps;
        }

        struct game_state new_state = current;

        if (current.empty_row < 3) {
            new_state = current;
            move_up(&new_state);  
            enqueue(&q, new_state);
        }

        if (current.empty_row > 0) {
            new_state = current;
            move_down(&new_state);  
            enqueue(&q, new_state);
        }

        if (current.empty_col < 3) {
            new_state = current;
            move_left(&new_state);
            enqueue(&q, new_state);
        }

        if (current.empty_col > 0) {
            new_state = current;
            move_right(&new_state);
            enqueue(&q, new_state);
        }
    }
    
    return -1;
}
