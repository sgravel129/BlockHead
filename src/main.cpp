#include "game.hpp"
// testing
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
//test
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