/*! Game module test cases */

#include "game.h"
#include "test_utils.h"

#include <stdio.h>

char level_directory[256];

void test_board_allocation()
{
    struct
    {
        int rows;
        int cols;
        cell cells[256];
    } test_vectors[] = { { 3, 4, { CELL_OPEN, CELL_OPEN, CELL_WALL, CELL_OPEN,
                                   CELL_WALL, CELL_SNAKE, CELL_SNAKE, CELL_SNAKE,
                                   CELL_WALL, CELL_FOOD, CELL_OPEN, CELL_WALL,
                                   CELL_OPEN, CELL_WALL, CELL_WALL, CELL_WALL } },

                         { 2, 5, { CELL_SNAKE, CELL_SNAKE, CELL_SNAKE, CELL_SNAKE, CELL_SNAKE,
                                   CELL_OPEN, CELL_WALL, CELL_OPEN, CELL_FOOD, CELL_FOOD } },

                         { 6, 1, { CELL_OPEN, CELL_WALL, CELL_SNAKE, CELL_WALL, CELL_OPEN, CELL_OPEN } } };

    int num_vectors = sizeof(test_vectors) / sizeof(test_vectors[0]);
    board * boards[num_vectors];
    int index;
    for (index = 0; index < num_vectors; index++)
    {
        printf("Board test %d\n", index);
        board * cur_board = boards[index] = create_board(test_vectors[index].rows, test_vectors[index].cols);
        test_result_pointer("create_board", cur_board);
        
        int cell_index;
        for (cell_index = 0; cell_index < test_vectors[index].rows * test_vectors[index].cols; cell_index++)
        {
            cur_board->cells[cell_index] = test_vectors[index].cells[cell_index];
        }
        
        cell * expected_cell = test_vectors[index].cells;
        int row, col;
        for (row = 0; row < test_vectors[index].rows; row++)
        {
            printf("Testing board row %d\n", row);
            for (col = 0; col < test_vectors[index].cols; col++)
            {
                cell cur_cell = *board_cell(cur_board, row, col);
                test_int("Board cell", cur_cell, *expected_cell);
                expected_cell++;
            }
        }
        printf("\n");
    }

    printf("Testing destroy_board for all created boards\n");
    printf("Valgrind will indicate if there is a problem in the deallocation\n");
    for (index = 0; index < num_vectors; index++)
    {
        destroy_board(boards[index]);
    }
    printf("\n\n");
}

void test_randomly_add_food()
{
    /* Make sure your tests have a high enough probability of food showing up
       to avoid the test failing due to random variation */
    struct
    {
        float probability;
        int rows;
        int cols;
        int open_cells;
    } test_vectors[] = { { 0.0, 10, 10, 90 },
                         { 1.0, 5, 10, 35 },
                         { 0.5, 10, 5, 50 },
                         { 0.8, 8, 8, 40 },
                         { 0.2, 4, 12, 48 } };

    int num_vectors = sizeof(test_vectors) / sizeof(test_vectors[0]);
    int index;
    for (index = 0; index < num_vectors; index++)
    {
        printf("randomly_add_food test %d\n", index);
        int iterations = 65536;
        board cur_board = { .rows = test_vectors[index].rows,
                            .cols = test_vectors[index].cols };
        cell board_data[cur_board.rows * cur_board.cols];
        cur_board.cells = board_data;
        float expected_new_food = test_vectors[index].probability * (float)test_vectors[index].open_cells /
                                  (float)test_vectors[index].rows / (float)test_vectors[index].cols * (float)iterations;
        int new_food = 0;
        int iteration;
        for (iteration = 0; iteration < iterations; iteration++)
        {
            int cell_index;
            for (cell_index = 0; cell_index < cur_board.rows * cur_board.cols; cell_index++)
            {
                if (cell_index < test_vectors[index].open_cells)
                {
                    cur_board.cells[cell_index] = CELL_OPEN;
                }
                else
                {
                    cur_board.cells[cell_index] = CELL_WALL;
                }
            }
            
            randomly_add_food(&cur_board, test_vectors[index].probability);
            
            int food_added = 0;
            for (cell_index = 0; cell_index < test_vectors[index].open_cells; cell_index++)
            {
                if (cur_board.cells[cell_index] == CELL_FOOD)
                {
                    food_added++;
                    new_food++;
                }
            }
            if (food_added != 0 && food_added != 1)
            {
                custom_test("food_added == 0 || food_added == 1", false);
            }
            
            int food_in_walls = 0;
            for (cell_index = test_vectors[index].open_cells; cell_index < cur_board.rows * cur_board.cols; cell_index++)
            {
                if (cur_board.cells[cell_index] == CELL_FOOD)
                {
                    food_in_walls++;
                }
            }
            if (food_in_walls != 0)
            {
                test_int("food in walls", food_in_walls, 0);
            }
        }
        test_float("Food added after 65536 tests", new_food, expected_new_food, 0.04);
    }
    printf("\n\n");
}

void test_snake_movement()
{
    struct
    {
        int rows;
        int cols;
        cell start_cells[64];
        int initial_row;
        int initial_col;
        int initial_growth;
        int growth_per_food;
        direction headings[8];
        cell end_cells[64];
    } test_vectors[] = { { 4, 3, { CELL_OPEN, CELL_OPEN, CELL_OPEN,
                                   CELL_OPEN, CELL_OPEN, CELL_OPEN,
                                   CELL_OPEN, CELL_OPEN, CELL_OPEN,
                                   CELL_OPEN, CELL_OPEN, CELL_OPEN },
                           1, 1, 0, 2, { NORTH, EAST, SOUTH, WEST, NORTH, WEST, SOUTH, EAST },
                                 { CELL_OPEN, CELL_OPEN, CELL_OPEN,
                                   CELL_OPEN, CELL_SNAKE, CELL_OPEN,
                                   CELL_OPEN, CELL_OPEN, CELL_OPEN,
                                   CELL_OPEN, CELL_OPEN, CELL_OPEN } },

                         { 3, 3, { CELL_OPEN, CELL_OPEN, CELL_OPEN,
                                   CELL_OPEN, CELL_WALL, CELL_OPEN,
                                   CELL_OPEN, CELL_WALL, CELL_OPEN },
                           0, 0, 2, 5, { EAST, SOUTH, EAST, SOUTH, WEST, SOUTH, NORTH, NORTH },
                                 { CELL_OPEN, CELL_OPEN, CELL_SNAKE,
                                   CELL_OPEN, CELL_WALL, CELL_OPEN,
                                   CELL_OPEN, CELL_WALL, CELL_OPEN } },

                         { 2, 5, { CELL_OPEN, CELL_OPEN, CELL_FOOD, CELL_OPEN, CELL_OPEN,
                                   CELL_OPEN, CELL_OPEN, CELL_FOOD, CELL_OPEN, CELL_OPEN },
                           1, 1, 0, 2, { EAST, NORTH, EAST, EAST, SOUTH, WEST, WEST, WEST },
                                 { CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_SNAKE,
                                   CELL_OPEN, CELL_SNAKE, CELL_SNAKE, CELL_SNAKE, CELL_SNAKE } },

                         { 2, 5, { CELL_OPEN, CELL_OPEN, CELL_WALL, CELL_OPEN, CELL_OPEN,
                                   CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_OPEN },
                           0, 1, 3, 3, { EAST, SOUTH, EAST, NORTH, EAST, WEST, WEST, SOUTH },
                                 { CELL_OPEN, CELL_OPEN, CELL_WALL, CELL_OPEN, CELL_OPEN,
                                   CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_OPEN, CELL_OPEN } } };

    int num_vectors = sizeof(test_vectors) / sizeof(test_vectors[0]);
    int steps = sizeof(test_vectors[0].headings) / sizeof(test_vectors[0].headings[0]);
    
    int index;
    snake * snakes[num_vectors];
    for (index = 0; index < num_vectors; index++)
    {
        printf("Snake movement test %d\n", index);
        board cur_board = { .rows = test_vectors[index].rows,
                            .cols = test_vectors[index].cols,
                            .cells = test_vectors[index].start_cells };
        snake * cur_snake = snakes[index] = create_snake(&cur_board, test_vectors[index].initial_row, test_vectors[index].initial_col,
                                                         test_vectors[index].headings[0], test_vectors[index].initial_growth);
        test_result_pointer("create_snake", cur_snake);
        int step;
        for (step = 0; step < steps; step++)
        {
            cur_snake->heading = test_vectors[index].headings[step];
            update_snake_head(cur_snake, &cur_board, test_vectors[index].growth_per_food);
            update_snake_tail(cur_snake, &cur_board);
        }
        
        board expected_board = { .rows = test_vectors[index].rows,
                                 .cols = test_vectors[index].cols,
                                 .cells = test_vectors[index].end_cells };
        
        test_board("board result", &cur_board, &expected_board);
    }
    
    printf("Testing destroy_snake for all created snakes.\n");
    printf("Valgrind will indicate if there is a problem with the deallocation.\n");
    for (index = 0; index < num_vectors; index++)
    {
        destroy_snake(snakes[index]);
    }
    printf("\n\n");
}

int main(int argc, char * argv[])
{
    test_function * functions[] = { test_board_allocation, test_randomly_add_food, test_snake_movement, NULL };
    return run_tests(functions);
}
