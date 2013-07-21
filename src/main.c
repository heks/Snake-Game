/*! Main module

    This is the entry point for the snake program.  It handles any command
    line arguments, sets up the game and view, and then passes off control
    to the view module.
    
    **You should not need to modify the code in this file**
*/

#include "view.h"
#include "file.h"

#include <libgen.h>

int main(int argc, char * argv[])
{
    
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    

    snake_view view = {};
    const char * level_file = argv[1];
    view.game = create_game(level_file);
    if (view.game == NULL)
    {
        fprintf(stderr, "Failed to load level file \"%s\"\n", level_file);
        return -1;
    }

    /* Hack to find resource directory relative to executable.
       I don't think there's a better way of doing this short of
       having the user specify it. */
    char * executable_directory = dirname(argv[0]);
    char resource_directory[128];
    snprintf(resource_directory, sizeof(resource_directory),
             "%s/../graphics", executable_directory);
    if (view_setup(&view, resource_directory))
    {
        return -1;
    }

    if (view_run(&view))
    {
        return -1;
    }

    view_teardown(&view);
    destroy_game(view.game);
    return 0;
}
