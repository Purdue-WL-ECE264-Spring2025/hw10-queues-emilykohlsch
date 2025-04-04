#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>


void enqueue(struct queue *q, struct game_state state) {
  size_t serialized = serialize(state);
  insert_at_tail(&q->data, serialized);
}

struct game_state dequeue(struct queue *q) { 
   if (!q->data.head) return (struct game_state){0}; 
   
   size_t serialized = remove_from_head(&q->data);
   return deserialize(serialized);
}

int number_of_moves(struct game_state start) { 
   struct queue q = {0};
    enqueue(&q, start);

    size_t visited[1000000] = {0};

    while (q.data.head) { 
        struct game_state current = dequeue(&q);

        int correct_value = 1;
        int solved = 1; 

        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (row == 3 && col == 3) {
                    if (current.tiles[row][col] != 0) {
                        solved = 0;
                    }
                } else {
                    if (current.tiles[row][col] != correct_value) {
                        solved = 0;
                    }
                    correct_value++;
                }
            }
        }
        if (solved) {
            return current.num_steps;
        }
        struct game_state neighbors[4] = {current, current, current, current};

        move_up(&neighbors[0]);
        move_down(&neighbors[1]);
        move_left(&neighbors[2]);
        move_right(&neighbors[3]);

        for (int i = 0; i < 4; i++) {
            size_t serialized = serialize(neighbors[i]);

            if (visited[serialized] == 0) {
                visited[serialized] = 1;
                enqueue(&q, neighbors[i]);
            }
        }
    }

    return -1;  
}
