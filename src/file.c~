/*! File module

    Provides level loading functionality through a function that creates a game from a
    level file.  Also provides a function for destroying the game instance for use during
    teardown.
*/

#include "game.h"
#include "file.h"

#include <stdio.h>
#include <stdlib.h>

game * create_game(const char * file_name)
/*! Load an input level file with given file_name with format specified in levels/README.txt,
    and transform the serialized data within into a game data structure, specified in game.h
    Return a pointer to the game instance on success, NULL on failure.

    Note that error handling is not required for credit, but may help with testing.

    The game and structures it points to should be allocated with the malloc family of
    functions.  Make sure to take advantage of allocation functions provided by the game module.
*/
{
    FILE *infile = fopen( file_name , "r" );   
    game *pGame = (game *)malloc( sizeof(game) );
    int rows, cols;
    
    int j, i, human_snake_row, human_snake_col, computer_snake_col, computer_snake_row;   
    char character;
    cell *pCell;
    direction human_snake;
    direction computer_snake;
    
    if(infile == NULL)
        return NULL;
      
    fscanf(infile, "%d %d %f %f %d\n", &rows, &cols, &(pGame->time_step), 
                                  &(pGame->food_probability), &(pGame->growth_per_food));   
	
	/*link the the board pointer to the game structure */
    pGame->board = create_board(rows, cols);
    
	
    if(pGame->board==NULL)
	    return NULL;
    
    for(j = 0; j < rows; j++)
    {
        for(i = 0; i < cols; i++)
        {
  	    pCell = board_cell(pGame->board, j, i);
            character = fgetc(infile);
	    
			switch(character)
			{
				case '-':
					*pCell = CELL_OPEN;
					break;
				case 'X': 
					*pCell = CELL_WALL;
					break;        	    
				case 'n':
					human_snake = NORTH;
					human_snake_col = i;
					human_snake_row = j;
					*pCell = CELL_SNAKE;
					break;  
				case 'e':
					human_snake = EAST;
					human_snake_col = i;
					human_snake_row = j;
					*pCell = CELL_SNAKE;
					break;  
				case 's':
					human_snake = SOUTH;
					human_snake_col = i;
					human_snake_row = j;
					*pCell = CELL_SNAKE;
					break;  
				case 'w':
					human_snake = WEST;
					human_snake_col = i;
					human_snake_row = j;
					*pCell = CELL_SNAKE;
					break;  
				case 'N':
					computer_snake = NORTH;
					computer_snake_col = i;
					computer_snake_row = j;
					*pCell = CELL_SNAKE;
					break;
				case 'E':
					computer_snake = EAST;
					computer_snake_col = i;
					computer_snake_row = j;
					*pCell = CELL_SNAKE;
					break;
				case 'S':
					computer_snake = SOUTH;
					computer_snake_col = i;
					computer_snake_row = j;
					*pCell = CELL_SNAKE;
					break;
				case 'W':
					computer_snake = WEST;
					computer_snake_col = i;
					computer_snake_row = j;
					*pCell = CELL_SNAKE;
					break;
				default :
					break;
			}
		}
	/*for new line character after end of column */
	fgetc(infile);
    }
    
    fclose(infile);  	

    /*human snake */
    pGame->human_snake = create_snake( pGame->board, human_snake_row, human_snake_col,
                                                                   human_snake, pGame->growth_per_food);
    
    /*computer snake */
    pGame->computer_snake = create_snake( pGame->board, computer_snake_row, computer_snake_col,
                                                                  computer_snake, pGame->growth_per_food);

    return pGame;
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains.  Make sure to take
    advantage of deallocation functions provided by the game module. */
{
 
    destroy_board(cur_game->board);
    destroy_snake(cur_game->human_snake);
    destroy_snake(cur_game->computer_snake);
    free(cur_game);
}
