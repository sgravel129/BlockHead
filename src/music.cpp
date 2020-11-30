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
    introMusic = Mix_LoadMUS("res/music/intro.wav");
    if (loopMusic == NULL)
    {
        Log::error("Failed to load intro music");
        success = false;
    }
    loopMusic = Mix_LoadMUS("res/music/loop.wav");
    if (loopMusic == NULL)
    {
        Log::error("Failed to load loop music");
        success = false;
    }
  

    return success;
}

void close() {
    //Free the music
    Mix_FreeMusic(introMusic);
    Mix_FreeMusic(loopMusic);
    introMusic = NULL;
    loopMusic = NULL;
    //Quit SDL subsystems
    Mix_Quit();
}