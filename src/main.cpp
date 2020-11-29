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

    try
    {
        Game game;

        game.setFramerate(framerate);


        game.start_menu();
        if (game.running()){
            game.map_selector_menu();
            game.run();
        }
        game.exit();
    }
    catch(std::runtime_error &err)
    {
        std::cerr << err.what() << std::endl;
    }

    
    return 0;
}
