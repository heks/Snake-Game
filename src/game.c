/*! Game module

    Implements the snake game, which consists of snake head and tail movement and
    random addition of food to the board by providing a function (next_frame) that
    performs one snake game iteration.
    Also provides functions for creating, destroying, and accessing game elements.
*/

#include "game.h"

#include <stdlib.h>

cell * board_cell(board * cur_board, int row, int col)
/*! Given a board, a row, and a column, return a pointer to the corresponding
    cell on the board. (See game.h for board data structure specification)
    This function should be handy for accessing board cells.
*/
{
    /* Implement me!  */
return (cell *)(cur_board->cells)+col+row*(cur_board->cols);   
}

void randomly_add_food(board * cur_board, float probability)
/*! Given a board and a probability, perform a random binary test
    that passes with the given probability (the random function will
    be useful for this, see "random" man page).  If the test passes, select
    a random cell on the board (each cell should have an equal chance
    of being selected) and change that cell to food if it is currently
    an open cell.  If the selected cell is not open, do nothing.

    We recommend that the default seed be used with the random number
    library, in order to produce deterministic results that make bugs
    easier to track down.  (i.e. just use random without calling srandom)
*/
{
    /* get random number from 0 to RAND_MAX*/
    long unsigned int random_number = random() % RAND_MAX;
    int status =0;
    /* if its less than the N*P the probability set status to 1 */
    if(random_number < RAND_MAX*probability) 
    	status = 1;
    /* if test passes select a random cell between 0 and (rows*cols)-1 */
    if(status)  
    {
 	random_number %= ((cur_board->rows)*(cur_board->cols));
	/*if that cell is OPEN put FOOD in it */
	if( *(cur_board->cells+random_number) == CELL_OPEN )
	    *(cur_board->cells+random_number) = CELL_FOOD; 
    }

	
}

board * create_board(int rows, int cols)
/*! Create an instance of a board structure with the given number of rows
    and columns, initializing elements to open cells and return a pointer
    to it. (See game.h for the specification for the board data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.

    Note that the calloc function is useful for allocating and zeroing
    memory in one call (see "calloc" man page)
*/
{   
    board *pBoard = malloc(sizeof(board));
    pBoard->rows = rows;
    pBoard->cols = cols;
    pBoard->cells = calloc( rows*cols, sizeof(cell) );
    return pBoard;
}

void destroy_board(board * cur_board)
/*! Free memory allocated by create_board to create the given board instance. */
{
    free(cur_board->cells);
    free(cur_board);
}

snake * create_snake(board * cur_board, int row, int col, direction heading, int initial_growth)
/*! Create a single-segment snake (See game.h for data structure specification)
    at the given row and column, with the given heading and given growth.
    Needed memory should be dynamically allocated with the malloc family of
    functions.  Mark the corresponding cell on the given board as occupied by
    a snake, and return a pointer to the newly created snake instance */
{
    /* This code is complete; you should not need to modify it */
    snake_segment * segment = malloc(sizeof(snake_segment));
    segment->row = row;
    segment->col = col;
    segment->next = NULL;

    snake * new_snake = malloc(sizeof(snake));
    new_snake->head = segment;
    new_snake->tail = segment;
    new_snake->heading = heading;
    new_snake->growth = initial_growth;

    *board_cell(cur_board, row, col) = CELL_SNAKE;
    return new_snake;
}

void destroy_snake(snake * cur_snake)
/*! Free memory associated with the given snake instance. */
{

    snake_segment *removed = NULL;
    while(cur_snake->head != NULL)
    {
	removed = cur_snake->head;
	cur_snake->head = removed->next;
	free(removed);
    }
    free(cur_snake);
}

void append_snake_head(snake * cur_snake, board * cur_board, int row, int col)
/*! Add a new snake segment to the given snake with the given row and column.
    This segment is the new snake head; update the snake data structure to integrate
    the new segment.  Needed memory should be allocated with the malloc family of
    functions.  Also mark the corresponding cell on the given board as being occupied
    by a snake.
*/
{
    snake_segment *pSeg = malloc(sizeof(snake_segment));
    pSeg->row = row;
    pSeg->col = col;
    pSeg->next = NULL;

    /*previous head snake point to the new snake head */
    cur_snake->head->next = pSeg;
    /* set the new head */
    cur_snake->head = pSeg;

    cell *pCell = board_cell(cur_board, row, col);
    *pCell = CELL_SNAKE;
}

void remove_snake_tail(snake * cur_snake, board * cur_board)
/*! Mark the cell on the board corresponding to the snake's last segment
    as open and remove the last segment from the snake.  Update the snake
    data structure to reflect this removal.
    
    If this removal results in the snake having no segments, then set the
    snake's tail and head to NULL to indicate a nonexistent snake.
*/
{
    /*mark corresponding cell to open */
    cell *pCell = board_cell(cur_board, cur_snake->tail->row, cur_snake->tail->col);
    *pCell = CELL_OPEN;   
    
    /* store the 2nd to last piece of the snake which will be the new tail */
    snake_segment *pSeg = cur_snake->tail->next;
    /* free the current tail */
    free(cur_snake->tail);
    /* put in the new tail */
    cur_snake->tail = pSeg;

    if(pSeg == NULL)
	cur_snake->head = NULL;
	
}

void update_snake_head(snake * cur_snake, board * cur_board, int growth_per_food)
/*! Attempt to move the head of the given snake one cell (by appending to the snake)
    according to the snake's heading (north is up).  The head can move in the given
    direction if the destination cell is either open or food.  If the cell is food,
    add the given growth per food to the snake's growth counter.  If the destination
    cell is a snake or wall, do nothing.
*/
{

/* do nothing if it has no pieces */
   if(cur_snake->head == NULL)
	{
	}
   else
   {
       int col = cur_snake->head->col;
       int row = cur_snake->head->row;
	

       switch(cur_snake->heading)
       {
        case NORTH:
	    row--;
	    break;
	case SOUTH:
 	    row++;
	    break;
	case EAST:
	    col++;
	    break;
	case WEST:
	    col--;
	    break;
        }

	
      if( *(board_cell(cur_board, row, col)) == CELL_OPEN || *(board_cell(cur_board, row, col)) == CELL_FOOD )
      {	
          if( *(board_cell(cur_board, row, col)) == CELL_FOOD )
	          cur_snake->growth += growth_per_food;

          append_snake_head(cur_snake, cur_board, row, col);	
      }  
    }	
	
}

void update_snake_tail(snake * cur_snake, board * cur_board)
/*! If the given snake's growth counter is 0, advance the tail of the
    given snake one cell (by removing the current snake tail).
    If the snake's growth counter is positive, decrement the growth
    counter and do not move the snake's tail.
*/
{
    /* do nothing if it has no pieces */
    if(cur_snake->tail == NULL)
	{
	}

    else
    {
    	if(cur_snake->growth == 0)
            remove_snake_tail(cur_snake, cur_board);

        else if( cur_snake->growth > 0 )
	    cur_snake->growth--;
    }

}

int next_frame(game * cur_game)
/*! Modify the state of the given snake game to reflect one game iteration.
    This consists of moving human and computer snakes and randomly adding food to the board.
*/
{
    /* This code is complete; you should not need to modify it */
    /* The computer snake gets priority (House always wins ties) */

    update_snake_head(cur_game->computer_snake, cur_game->board, cur_game->growth_per_food);
    update_snake_head(cur_game->human_snake, cur_game->board, cur_game->growth_per_food);
    update_snake_tail(cur_game->computer_snake, cur_game->board);
    update_snake_tail(cur_game->human_snake, cur_game->board);
    randomly_add_food(cur_game->board, cur_game->food_probability);
    return 0;
}
