#pragma once

#include "game.h"

game * create_game(const char * file_name);
void destroy_game(game * cur_game);
