/*! File module test cases

    Provides tests for loading levels from a file.
*/

#include "file.h"
#include "test_utils.h"

#include <stdio.h>
#include <libgen.h>

char level_directory[256];
float error_tolerance = 1e-4;

void test_create_destroy_game()
{
    struct
    {
        char * level_file;
        struct
        {
            int rows;
            int cols;
            float time_step;
            float food_probability;
            int growth_per_food;
            int human_snake_row;
            int human_snake_col;
            direction human_snake_heading;
            int computer_snake_row;
            int computer_snake_col;
            direction computer_snake_heading;
        } expected;
        struct
        {
            int row;
            int col;
            cell value;
        } cell_tests[4];
    } test_vectors[] = { { "maze.txt", { 13, 49, 0.25, 0.4, 5, 11, 1, NORTH, 1, 47, WEST },
                                       { { 0, 0, CELL_WALL }, { 12, 48, CELL_WALL }, { 6, 14, CELL_WALL }, { 8, 29, CELL_OPEN } } },
                         { "simple.txt", { 30, 40, 0.2, 0.1, 8, 28, 18, NORTH, 1, 19, SOUTH },
                                         { { 20, 0, CELL_WALL }, { 28, 18, CELL_SNAKE }, { 1, 19, CELL_SNAKE }, { 1, 2, CELL_OPEN } } },
                         { "small.txt", { 18, 24, 0.25, 0.1, 5, 10, 1, EAST, 1, 10, SOUTH },
                                        { { 10, 1, CELL_SNAKE }, { 7, 10, CELL_OPEN }, { 16, 1, CELL_OPEN }, { 16, 23, CELL_WALL } } },
                         { "random.txt", { 30, 30, 0.2, 0.1, 8, 15, 28, WEST, 15, 1, EAST },
                                         { { 10, 1, CELL_WALL }, { 29, 20, CELL_WALL }, { 28, 28, CELL_WALL }, { 14, 20, CELL_OPEN } } } };

    int num_vectors = sizeof(test_vectors) / sizeof(test_vectors[0]);
    game * games[num_vectors];

    int index;
    for (index = 0; index < num_vectors; index++)
    {
        printf("Testing create_game with level \"%s\"\n", test_vectors[index].level_file);
        char level_file[256];
        snprintf(level_file, sizeof(level_file), "%s/%s", level_directory, test_vectors[index].level_file);
        game * new_game = games[index] = create_game(level_file);
        test_result_pointer("create_game", new_game);
        test_int("Level rows", new_game->board->rows, test_vectors[index].expected.rows);
        test_int("Level cols", new_game->board->cols, test_vectors[index].expected.cols);
        test_float("Level time step", new_game->time_step, test_vectors[index].expected.time_step, error_tolerance);
        test_float("Level food probability", new_game->food_probability, test_vectors[index].expected.food_probability, error_tolerance);
        test_int("Level growth per food", new_game->growth_per_food, test_vectors[index].expected.growth_per_food);
        test_int("Level human snake row", new_game->human_snake->head->row, test_vectors[index].expected.human_snake_row);
        test_int("Level human snake col", new_game->human_snake->head->col, test_vectors[index].expected.human_snake_col);
        test_int("Level human snake heading", new_game->human_snake->heading, test_vectors[index].expected.human_snake_heading);
        test_int("Level computer snake row", new_game->computer_snake->head->row, test_vectors[index].expected.computer_snake_row);
        test_int("Level computer snake col", new_game->computer_snake->head->col, test_vectors[index].expected.computer_snake_col);
        test_int("Level computer snake heading", new_game->computer_snake->heading, test_vectors[index].expected.computer_snake_heading);

        int num_test_cells = sizeof(test_vectors[0].cell_tests) / sizeof(test_vectors[0].cell_tests[0]);
        int cell_index;
        for (cell_index = 0; cell_index < num_test_cells; cell_index++)
        {
            int row = test_vectors[index].cell_tests[cell_index].row;
            int col = test_vectors[index].cell_tests[cell_index].col;
            test_int("Level cell test", new_game->board->cells[row * test_vectors[index].expected.cols + col],
                                        test_vectors[index].cell_tests[cell_index].value);
        }
        printf("\n");
    }
    
    printf("Testing destory_game for all loaded levels\n");
    printf("Valgrind will indicate if there is a problem in the deallocation\n");
    for (index = 0; index < num_vectors; index++)
    {
        destroy_game(games[index]);
    }
}

int main(int argc, char * argv[])
{
    char * executable_directory = dirname(argv[0]);
    snprintf(level_directory, sizeof(level_directory),
             "%s/../levels", executable_directory);

    test_function * functions[] = { test_create_destroy_game, NULL };
    return run_tests(functions);
}
