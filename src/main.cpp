#include "game.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

int main(int argc, char **argv)
{
    int framerate = 60;

    // Command line arguements
    if (argc > 2)
    {
        framerate = atoi(argv[2]);
    }

    Game game;

    game.setFramerate(framerate);

    if (game.init())
    {
        game.run();
    }

    game.exit();

    return 0;
}