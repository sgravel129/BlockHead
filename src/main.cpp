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

<<<<<<< HEAD
    testMain();
    
    /*
    Game game;
=======
    try
    {
        Game game;
>>>>>>> origin/master

        game.setFramerate(framerate);

<<<<<<< HEAD
    ////////
    
    /////////

    /*
    if (game.init())
=======
        game.start_menu();
        if (game.running()){
            game.map_selector_menu();
            game.run();
        }
        game.exit();
    }
    catch(std::runtime_error &err)
>>>>>>> origin/master
    {
        std::cerr << err.what() << std::endl;
    }

<<<<<<< HEAD
    game.exit();
    */
    return 0;
}


=======

    return 0;
}
>>>>>>> origin/master
