#pragma once

#include "game.h"

#include </SDL/SDL.h>

#define ACTION_QUEUE_SIZE 4

typedef enum
{
    ACTION_NONE,
    ACTION_NORTH,
    ACTION_SOUTH,
    ACTION_EAST,
    ACTION_WEST,
} action;

typedef enum
{
    IMG_BLOB,
    IMG_NORTH,
    IMG_SOUTH,
    IMG_EAST,
    IMG_WEST,
    IMG_NW,
    IMG_SW,
    IMG_NE,
    IMG_SE,
    IMG_EW,
    IMG_NS,
    IMG_0,
    IMG_1,
    IMG_2,
    IMG_MAX,
} sprite_type;

typedef struct
{
    int start;
    int end;
    action actions[ACTION_QUEUE_SIZE];
} action_queue;

typedef struct
{
    game * game;
    SDL_Surface * screen;
    SDL_Surface * sprites[IMG_MAX];
    action_queue action_queue;
} snake_view;

int view_setup(snake_view * view, const char * resource_directory);
int view_teardown(snake_view * view);
int view_run(snake_view * view);
