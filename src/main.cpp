#include "game.hpp"
#include "testPath.hpp"


#include <iostream>

int main(int argc, char **argv)
{
    int framerate = 60;

    // Command line arguements
    if (argc > 2)
    {
        framerate = atoi(argv[2]);
    }

    testMain();
    
    /*
    Game game;

    game.setFramerate(framerate);

    ////////
    
    /////////

    /*
    if (game.init())
    {
        game.run();
    }

    game.exit();
    */
    return 0;
}


