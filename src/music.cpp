#include "music.hpp"

bool initMusic() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        Log::error("Failed to initialize SDL for music");
        success = false;
    }
    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        Log::error("Failed to initialize SDL Mixer");
        success = false;
    }
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;


    //Load music
    gIntroMusic = Mix_LoadMUS("res/music/gameIntro.wav");
    if (gIntroMusic == NULL)
    {
        Log::error("Failed to load intro music");
        success = false;
    }
    gLoopMusic = Mix_LoadMUS("res/music/gameLoop.wav");
    if (gLoopMusic == NULL)
    {
        Log::error("Failed to load loop music");
        success = false;
    }
    menuMusic = Mix_LoadMUS("res/music/menu.wav");
    if (menuMusic == NULL)
    {
        Log::error("Failed to load music");
        success = false;
    }
    gameoverMusic = Mix_LoadMUS("res/music/gameover.wav");
    if (gameoverMusic == NULL)
    {
        Log::error("Failed to load music");
        success = false;
    }
    wIntroMusic = Mix_LoadMUS("res/music/fanfareIntro.wav");
    if (wIntroMusic == NULL)
    {
        Log::error("Failed to load music");
        success = false;
    }
    wLoopMusic = Mix_LoadMUS("res/music/fanfareLoop.wav");
    if (wLoopMusic == NULL)
    {
        Log::error("Failed to load music");
        success = false;
    }
    

    return success;
}

void closeMusic() {
    //Free the music
    Mix_FreeMusic(gIntroMusic);
    Mix_FreeMusic(gLoopMusic);
    Mix_FreeMusic(menuMusic);
    Mix_FreeMusic(gameoverMusic);
    Mix_FreeMusic(wIntroMusic);
    Mix_FreeMusic(wLoopMusic);
    gIntroMusic = NULL;
    gLoopMusic = NULL;
    menuMusic = NULL;
    gameoverMusic = NULL;
    wIntroMusic = NULL;
    wLoopMusic = NULL;
    //Quit SDL subsystems
    Mix_Quit();
}