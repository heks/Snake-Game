#pragma once

#include <stdbool.h>

typedef enum
{
    CELL_OPEN,
    CELL_FOOD,
    CELL_WALL,
    CELL_SNAKE,
} cell;

typedef enum
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
} direction;

/*! A segment of a snake (each segment occupies a cell)
    This is like a node in a linked list. */
typedef struct snake_segment
{
    /* Row of the cell the segment occupies */
    int row;
    /* Column of the cell the segment occupies */
    int col;
    /* Pointer to the next segment, in the direction of snake movement */
    struct snake_segment * next;
} snake_segment;

/*! Defines a snake, which is like a linked-list with the
    head and tail reversed. */
typedef struct
{
    /* The front segment of the snake, or NULL if snake has no segments */
    snake_segment * head;
    /* The rear segment of the snake, or NULL if snake has no segments */
    snake_segment * tail;
    /* The direction the snake head is going to move on the next iteration */
    direction heading;
    /* Snake growth counter indicating how many segments the snake should add */
    int growth;
} snake;

/*! Defines a board, which is a just a grid of cells. */
typedef struct
{
    /* Number of rows in the grid */
    int rows;
    /* Number of columns in the grid */
    int cols;
    /* Pointer to the beginning of the board cell data.  The cells are organized
       in a row-major layout ( http://en.wikipedia.org/wiki/Row-major_order ) */
    cell * cells;
} board;

/*! Defines the game data structure, which consists of the board, the two snakes,
    and some game parameters. */
typedef struct
{
    /* The game board */
    board * board;
    /* The human controlled snake */
    snake * human_snake;
    /* The computer controlled snake */
    snake * computer_snake;
    /* Time delay between each game iteration, in seconds */
    float time_step;
    /* Probability of one food cell appeared in each game iteration */
    float food_probability;
    /* Segments that snakes grow for each food cell that they eat */
    int growth_per_food;
} game;

board * create_board(int rows, int cols);
cell * board_cell(board * cur_board, int row, int col);
void destroy_board(board * cur_board);

snake * create_snake(board * cur_board, int row, int col, direction heading, int initial_growth);
void destroy_snake(snake * cur_snake);
void update_snake_head(snake * cur_snake, board * cur_board, int growth_per_food);
void update_snake_tail(snake * cur_snake, board * cur_board);

void randomly_add_food(board * cur_board, float probability);

int next_frame(game * cur_game);
