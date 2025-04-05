#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>


void enqueue(struct queue *q, struct game_state state) {
  size_t encoded = serialize(state);
  insert_at_tail(&q->data, encoded);
}

int is_solved(struct game_state state) {
    int goal[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (state.tiles[r][c] != goal[r][c]) return 0;
        }
    }
    return 1;
}

struct game_state dequeue(struct queue *q) { 
   size_t encoded = remove_from_head(&q->data);  // uses linked_list function
   return deserialize(encoded);
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
