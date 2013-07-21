/*! AI module test cases */

#include "ai.h"
#include "test_utils.h"

#include <stdio.h>
#include <limits.h>

struct
{
    int rows;
    int cols;
    int initial_row;
    int initial_col;
    int expected_distance;
    direction expected_direction;
    cell cells[64];
} test_vectors[] = { { 5, 4, 1, 1, INT_MAX, SOUTH,
                     { CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, } },
                     { 5, 4, 2, 1, INT_MAX, NORTH,
                     { CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_WALL, CELL_WALL,
                       CELL_WALL, CELL_WALL, CELL_FOOD, CELL_WALL,
                       CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, } },
                     { 5, 6, 1, 1, 6, EAST,
                     { CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_OPEN, CELL_WALL, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_SNAKE,CELL_FOOD, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, } },
                     { 5, 6, 4, 3, 4, EAST,
                     { CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL,
                       CELL_WALL, CELL_FOOD, CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_WALL, CELL_FOOD, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_WALL, CELL_SNAKE,CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_OPEN, CELL_OPEN, CELL_WALL, CELL_OPEN, CELL_WALL,
                       CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, } } };

void test_find_food()
{
    int index, num_vectors = sizeof(test_vectors) / sizeof(test_vectors[0]);
    for (index = 0; index < num_vectors; index++)
    {
        printf("find_food test %d\n", index);
        board cur_board = { .rows = test_vectors[index].rows,
                            .cols = test_vectors[index].cols,
                            .cells = test_vectors[index].cells };
        int distance_map[64];
        int map_index;
        for (map_index = 0; map_index < 64; map_index++)
        {
            distance_map[map_index] = INT_MAX;
        }
        int closest_distance = INT_MAX;
        find_food(&cur_board, distance_map, test_vectors[index].initial_row,
                  test_vectors[index].initial_col, 0, &closest_distance);
        
        test_int("Food distance", closest_distance, test_vectors[index].expected_distance);
    }                    
}

void test_ai_move()
{    
    int index, num_vectors = sizeof(test_vectors) / sizeof(test_vectors[0]);
    for (index = 0; index < num_vectors; index++)
    {
        board cur_board = { .rows = test_vectors[index].rows,
                            .cols = test_vectors[index].cols,
                            .cells = test_vectors[index].cells };
        snake_segment segment = { .row = test_vectors[index].initial_row,
                                  .col = test_vectors[index].initial_col };
        snake cur_snake = { .head = &segment,
                            .tail = &segment };
        game cur_game = { .board = &cur_board,
                          .computer_snake = &cur_snake };
        
        ai_move(&cur_game);
        test_int("AI's move", cur_game.computer_snake->heading,
                              test_vectors[index].expected_direction);
    }
}

int main(int argc, char * argv[])
{
    test_function * functions[] = { test_find_food, test_ai_move, NULL };
    return run_tests(functions);
}
