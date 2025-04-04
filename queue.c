#include "queue.h"
#include "tile_game.h"

#define VISITED_SIZE 2000003 // prime number closest to a millon for possible tile arrangements so that collisions will be reduced, the bigger the number the more memory used


//this will insert at the tail
void enqueue(struct queue *q, struct game_state state)
{
    // this seriliazes the game state into a uint64_t
    uint64_t serialized_state = serialize(state);

    // then it insterts the serialized state at the tail of the linked list
    insert_at_tail(&q->data, (size_t)serialized_state);

}

//this removes at the head
struct game_state dequeue(struct queue *q)
{
    if (q->data.head == NULL) {return (struct game_state){0}; }// returns a default-initialized game_state if there is no head in the data

    // this removes the head of the linked list and then gets the serialized state
    uint64_t serialized_state = (uint64_t)remove_from_head(&q->data);
    struct game_state result;

    // this deserilazes the state back into a game_state struct
    result = deserialize(serialized_state);
    return result;
}

int number_of_moves(struct game_state start)
{ 
    
    struct queue q = {0};
    enqueue(&q, start);

    // this is the target tile configuration that is solved
    uint8_t solved[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};

    // this tracks which tile has been visisted (critical for performance)
    bool visited[VISITED_SIZE] = {false};
    visited[serialize(start) % VISITED_SIZE] = true; // we start as visited is true

    while (q.data.head != NULL) 
    {
        bool is_solved;

        // we start by removing the head of the list
        struct game_state current_game = dequeue(&q);
        
        // we check if solved in a optimized way (optimized)
        is_solved = true;

        for (int i = 0; i < 4; i++) 
        {
            for (int j = 0; j < 4; j++) 
            {
                if (current_game.tiles[i][j] != solved[i][j]) {is_solved = false; break;}
            }
            if (!is_solved) break; 
        }
        
        if (is_solved) {free_list(q.data); return current_game.num_steps;} // once it's solved to how it's supposed to be solved we return the number of steps

        // Now we have to find the number of steps taken so that we update the Current struct
        int movement[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

        for (int i = 0; i < 4; i++) 
        {
            int new_row;
            int new_col;

            new_row = current_game.empty_row + movement[i][0];
            new_col = current_game.empty_col + movement[i][1];
            
            if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4) 
            {
                struct game_state next = current_game;

                next.tiles[current_game.empty_row][current_game.empty_col] = next.tiles[new_row][new_col];
                next.tiles[new_row][new_col] = 0;
                next.empty_row = new_row;
                next.empty_col = new_col;
                next.num_steps++;
                
                // so we check if it's been visited before we enqueue
                uint64_t key = serialize(next);
                if (!visited[key % VISITED_SIZE]){visited[key % VISITED_SIZE] = true; enqueue(&q, next);}
            }
        }
    }
    return -1;  // this is for if the puzzle is unsolvable/unreachable
}
