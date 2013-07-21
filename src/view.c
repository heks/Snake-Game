/*! View module

    Handles loading sprites, getting keyboard input, and drawing the board and snakes to the screen.
    
    **You should not need to modify the code in this file**
*/

#include "view.h"
#include "ai.h"

#include <math.h>
#include <unistd.h>
#include <sys/time.h>

const int secs_in_usec = 1e6;
const int ui_poll_interval = 1e-3;
const int sprite_size = 16;

int load_images(snake_view * view, const char * resource_directory)
/*! Load the game sprites from the given resource directory */
{
    struct
    {
        sprite_type identifier;
        const char * filename;
    }
    file_map[] = { { IMG_BLOB, "blob.bmp" },
                   { IMG_NORTH, "n.bmp" },
                   { IMG_SOUTH, "s.bmp" },
                   { IMG_EAST, "e.bmp" },
                   { IMG_WEST, "w.bmp" },
                   { IMG_NW, "nw.bmp" },
                   { IMG_SW, "sw.bmp" },
                   { IMG_NE, "ne.bmp" },
                   { IMG_SE, "se.bmp" },
                   { IMG_EW, "ew.bmp" },
                   { IMG_NS, "ns.bmp" },
                   { IMG_0, "0.bmp" },
                   { IMG_1, "1.bmp" },
                   { IMG_2, "2.bmp" } };

    int index;
    for (index = 0; index < sizeof(file_map) / sizeof(file_map[0]); index++)
    {
        char resource_path[128];
        snprintf(resource_path, sizeof(resource_path),
                 "%s/%s", resource_directory, file_map[index].filename);
        SDL_Surface * image = SDL_LoadBMP(resource_path);
        if (image == NULL)
        {
            fprintf(stderr, "Failed to load \"%s\": %s\n", file_map[index].filename, SDL_GetError());
            return -1;
        }
        view->sprites[file_map[index].identifier] = image;
    }
    return 0;
}

int view_setup(snake_view * view, const char * resource_directory)
/*! Setup the SDL viewport, and do any resource loading needed by the game. */
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    view->screen = SDL_SetVideoMode(view->game->board->cols * sprite_size,
                                    view->game->board->rows * sprite_size, 32, SDL_SWSURFACE);
    if (view->screen == NULL)
    {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return -1;
    }

    if (load_images(view, resource_directory))
    {
        fprintf(stderr, "Unable to load sprites!\n");
        return -1;
    }

    SDL_WM_SetCaption("Snake", "Snake");
    return 0;
}

int view_teardown(snake_view * view)
/*! Do any cleanup needed before the view exits. */
{
    sprite_type sprite;
    for (sprite = 0; sprite < IMG_MAX; sprite++)
    {
        SDL_FreeSurface(view->sprites[sprite]);
    }
    SDL_Quit();
    return 0;
}

int draw_sprite(snake_view * view, sprite_type sprite, int row, int col)
/*! Draw the given sprite to the given cell in the viewport */
{
    if (sprite_size * (col + 1) > view->screen->w || sprite_size * (row + 1) > view->screen->h)
    {
        return -1;
    }
    SDL_Rect rect = { sprite_size * col, sprite_size * row, sprite_size, sprite_size };
    SDL_BlitSurface(view->sprites[sprite], NULL, view->screen, &rect);
    return 0;
}

sprite_type endcap_sprite(snake_segment * from_segment, snake_segment * to_segment)
/*! Determine the appropriate encap sprite to use for the given
    contiguous end segments */
{
    if (from_segment->row < to_segment->row)
    {
        return IMG_NORTH;
    }
    if (from_segment->row > to_segment->row)
    {
        return IMG_SOUTH;
    }
    if (from_segment->col > to_segment->col)
    {
        return IMG_EAST;
    }
    if (from_segment->col < to_segment->col)
    {
        return IMG_WEST;
    }
    return IMG_BLOB;
}

sprite_type section_sprite(snake_segment * pivot, snake_segment * a, snake_segment * b)
/*! Determine the appropriate elbow or straight sprite to use for the
    given three contigous segments */
{
    if (a->row == b->row)
    {
        return IMG_EW;
    }
    if (a->col == b->col)
    {
        return IMG_NS;
    }
    if ((a->col < b->col) ^ (a->row < b->row))
    {
        if (a->col < pivot->col || b->col < pivot->col)
        {
            return IMG_NW;
        }
        else
        {
            return IMG_SE;
        }
    }
    else
    {
        if (a->col < pivot->col || b->col < pivot->col)
        {
            return IMG_SW;
        }
        else
        {
            return IMG_NE;
        }
    }
}

void draw_snake(snake_view * view, snake * cur_snake)
/*! Draw the given snake on top of the given viewport */
{
    if (cur_snake == NULL)
    {
        return;
    }
    snake_segment * prev_segment, * segment;
    for (prev_segment = NULL, segment = cur_snake->tail;
         segment; prev_segment = segment, segment = segment->next)
    {
        sprite_type sprite;
        if (prev_segment != NULL)
        {
            if (segment->next != NULL)
            {
                sprite = section_sprite(segment, prev_segment, segment->next);
            }
            else
            {
                sprite = endcap_sprite(prev_segment, segment);
            }
        }
        else
        {
            if (segment->next != NULL)
            {
                sprite = endcap_sprite(segment->next, segment);
            }
            else
            {
                sprite = IMG_BLOB;
            }
        }
        draw_sprite(view, sprite, segment->row, segment->col);
    }
}

void draw_board(snake_view * view, board * cur_board)
/*! Draw the given board to the given viewport */
{
    int row, col;
    for (row = 0; row < cur_board->rows; row++)
    {
        for (col = 0; col < cur_board->cols; col++)
        {
            sprite_type sprite;
            switch (*board_cell(cur_board, row, col))
            {
            case CELL_WALL:
                sprite = IMG_2;
                break;
            case CELL_OPEN:
                sprite = IMG_0;
                break;
            case CELL_FOOD:
                sprite = IMG_1;
                break;
            case CELL_SNAKE:
                sprite = IMG_BLOB;
                break;
            }
            draw_sprite(view, sprite, row, col);
        }
    }
}

void update_view(snake_view * view)
/*! Update the view to reflect the current game state */
{
    draw_board(view, view->game->board);
    draw_snake(view, view->game->human_snake);
    draw_snake(view, view->game->computer_snake);
    SDL_UpdateRect(view->screen, 0, 0, 0, 0);
}

double get_time()
/*! Get the current time in seconds with microsecond precision */
{
    struct timeval cur_time = {};
    gettimeofday(&cur_time, NULL);
    return (double)cur_time.tv_sec + (double)cur_time.tv_usec / secs_in_usec;
}

void queue_action(action_queue * queue, action new_action)
/*! Queue the given action into the action queue */
{
    if ((queue->end + 1) % ACTION_QUEUE_SIZE == queue->start)
    {
        return;
    }
    queue->actions[queue->end] = new_action;
    queue->end = (queue->end + 1) % ACTION_QUEUE_SIZE;
}

action dequeue_action(action_queue * queue)
/*! Dequeue the next action from the action queue */
{
    if (queue->start == queue->end)
    {
        return ACTION_NONE;
    }
    action cur_action = queue->actions[queue->start];
    queue->start = (queue->start + 1) % ACTION_QUEUE_SIZE;
    return cur_action;
}

int queue_input(action_queue * queue)
/*! Poll for any keyboard events, and queue any that correspond to snake
    movement commands.  Return -1 to signal that user wants to quit. */
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return -1;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
            case SDLK_w:
                queue_action(queue, ACTION_NORTH);
                break;
            case SDLK_DOWN:
            case SDLK_s:
                queue_action(queue, ACTION_SOUTH);
                break;
            case SDLK_LEFT:
            case SDLK_a:
                queue_action(queue, ACTION_WEST);
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                queue_action(queue, ACTION_EAST);
                break;
            case SDLK_q:
                return -1;
            default:
                break;
            }
        }
    }
    return 0;
}

int view_run(snake_view * view)
/*! The main event loop of the view.  Provides "glue code" that puts all of
    the above functionality together in a screen updating, action handling loop. */
{
    double deadline, cur_time;
    cur_time = get_time();
    while (true)
    {
        update_view(view);
        deadline = cur_time + view->game->time_step;
        ai_move(view->game);

        while (true)
        {
            if (queue_input(&view->action_queue))
            {
                return 0;
            }
            cur_time = get_time();
            if (cur_time >= deadline)
            {
                break;
            }
            usleep(fmin(ui_poll_interval, (deadline - cur_time) * secs_in_usec));
        }

        action cur_action = dequeue_action(&view->action_queue);
        switch (cur_action)
        {
        case ACTION_NORTH:
            view->game->human_snake->heading = NORTH;
            break;
        case ACTION_SOUTH:
            view->game->human_snake->heading = SOUTH;
            break;
        case ACTION_EAST:
            view->game->human_snake->heading = EAST;
            break;
        case ACTION_WEST:
            view->game->human_snake->heading = WEST;
            break;
        case ACTION_NONE:
            break;
        }
        next_frame(view->game);
    }
}
