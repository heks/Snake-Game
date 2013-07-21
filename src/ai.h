#pragma once

#include "game.h"

void find_food(board * cur_board, int * distance_map, int row, int col,
               int distance, int * closest_distance);
void ai_move(game * game);
