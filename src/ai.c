/*! AI module

    Implements the Artifical Intelligence of the computer controlled snake by
    providing a function that determines the computer's next move based on the
    current game state.
    
    The AI's goal is simply to find the shortest path to the nearest food, and to
    then follow that path.  If food is inaccessible, then the AI's goal is to avoid
    hitting walls.
*/

#include "ai.h"

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

void find_food(board * cur_board, int * distance_map, int row, int col,
               int distance, int * closest_distance)
/*! Given a board (data structure specified in game.h), a starting row and column,
    and a given path distance traveled, recursively search the board to find the
    shortest path to food.  The closest_distance input/output parameter indicates
    the closest distance to food found thus far.  When closer food is found, update
    closest_distance to reflect that.

    The distance map parameter is an array of integers corresponding to the board
    cell array.  In order for the search of large boards to finish in a reasonable
    amount of time, each entry in the distance map should be updated with the shortest
    path distance found to the corresponding cell thus far and a branch of the search
    should be ignored if it finds a path to a cell that is as long or longer than the
    shortest distance recorded in the distance map for that cell. (because that
    search is going to produce the same or worse results)
*/
{
    int *ptr = distance_map+col+(cur_board->cols*row);

    if(row >= 0 && row < cur_board->rows && col >= 0 && cur_board->cols > col)
    {
        if( *(board_cell(cur_board, row, col)) == CELL_OPEN || *(board_cell(cur_board, row, col)) == CELL_FOOD )
	{
	
	    if( distance < *ptr )
	    {
	
		*ptr = distance++;		
		
		if( *(board_cell(cur_board, row, col)) == CELL_FOOD && *ptr < *closest_distance )
		{
			*closest_distance = *ptr;
		}
	 
		find_food(cur_board, distance_map,row+1 ,col, distance, closest_distance);
		find_food(cur_board, distance_map,row-1,col, distance, closest_distance);
		find_food(cur_board, distance_map,row,col+1, distance, closest_distance);
		find_food(cur_board, distance_map,row,col-1, distance, closest_distance);
	    } 
	} 
    }   
 
}

int food_distance(board * cur_board, int * distance_map, int row, int col)
/*! Given a board (data structure defined in game.h), a distance map (format defined
    in find_food) which is only allocated, not initialized, and a starting row and
    column, return the closest path distance to food.
    If no food is found, return INT_MAX (defined in limits.h,
    see http://uw714doc.sco.com/en/man/html.4/limits.4.html )
*/
{
    /* This code is complete; you should not need to modify it */
    int index, distance = INT_MAX;
    for (index = 0; index < cur_board->rows * cur_board->cols; index++)
    {
        distance_map[index] = INT_MAX;
    }
    find_food(cur_board, distance_map, row, col, 0, &distance);
    return distance;
}

void avoid_walls(board * cur_board, snake * cur_snake)
/*! Given a board and a snake (see game.h for data structure definitions)
    modify the heading of the given snake to avoid crashing into walls or
    snakes, if possible.
    This function assumes that no food is in reach of the given snake.
*/
{
    /* This code is complete; you should not need to modify it */
    int row = cur_snake->head->row;
    int col = cur_snake->head->col;
    if (*board_cell(cur_board, row + 1, col) == CELL_OPEN)
    {
        cur_snake->heading = SOUTH;
    }
    else if (*board_cell(cur_board, row - 1, col) == CELL_OPEN)
    {
        cur_snake->heading = NORTH;
    }
    else if (*board_cell(cur_board, row, col + 1) == CELL_OPEN)
    {
        cur_snake->heading = EAST;
    }
    else if (*board_cell(cur_board, row, col - 1) == CELL_OPEN)
    {
        cur_snake->heading = WEST;
    }
    else
    {
        /* We're blocked off; nothing we can do */
    }
}

void ai_move(game * cur_game)
/*! Given a game state (game data structure defined in game.h), determine the
    computer's next move.  For each cell adjacent to the snake head, search for
    food.  If food is found, change the computer snake's heading to go to the cell
    that has the closest distance to food. (If two or more cells have the shortest
    distance, then you are free to choose one of them)

    If no food is found, then simply call the avoid_walls function.
*/
{

    /*get the row and col of the computer snakes head */

    int row = cur_game->computer_snake->head->row;
    int col = cur_game->computer_snake->head->col;

    int *distance_map= malloc(sizeof(int)*cur_game->board->rows*cur_game->board->cols);
  
    int d[4];
    int min;
    int counter;
   
    d[0] = food_distance(cur_game->board, distance_map, row+1, col);
    d[1] = food_distance(cur_game->board, distance_map, row-1, col);
    d[2] = food_distance(cur_game->board, distance_map, row, col+1);
    d[3] = food_distance(cur_game->board, distance_map, row, col-1);

    free(distance_map);

    /*find smallest distance */
    min = d[0];
    for(counter = 0; counter < 4; counter++)
    {
	if(min > d[counter] )
	    min = d[counter];
    }

    /*find which direction to go */
    if(min == INT_MAX)
        avoid_walls(cur_game->board, cur_game->computer_snake);
    else if(min == d[0])
        cur_game->computer_snake->heading = SOUTH; 
    else if(min == d[1])
        cur_game->computer_snake->heading = NORTH; 
    else if(min == d[2])
        cur_game->computer_snake->heading = EAST; 
    else if(min == d[3])
	cur_game->computer_snake->heading = WEST; 

}

