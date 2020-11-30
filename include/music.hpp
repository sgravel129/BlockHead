#pragma once

#if defined(_WIN32)
	#include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_mixer.h>
#else
	#include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_mixer.h>
#endif

#include <stdio.h>
#include <string>
#include "log.hpp"

bool initMusic();
bool loadMedia();
void close();

extern Mix_Music* introMusic;
extern Mix_Music* loopMusic;
