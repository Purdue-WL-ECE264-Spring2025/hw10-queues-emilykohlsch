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
int is_solved(struct game_state state) {
    int expected = 1;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (row == 3 && col == 3) {
                if (state.tiles[row][col] != 0) return 0;
            } else {
                if (state.tiles[row][col] != expected++) return 0;
            }
        }
    }
    return 1;
}
int number_of_moves(struct game_state start) { 
   struct queue q = {0};
   enqueue(&q, start);

   size_t visited[1000000] = {0}; 

   while (q.data.head) {
     struct game_state current = dequeue(&q);
       
     if (is_solved(current)) return current.num_steps;

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

    return -1; // No solution found
}
