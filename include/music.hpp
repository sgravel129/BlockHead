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

extern Mix_Music* gIntroMusic;
extern Mix_Music* gLoopMusic;
extern Mix_Music* menuMusic;
extern Mix_Music* gameoverMusic;
extern Mix_Music* wIntroMusic;
extern Mix_Music* wLoopMusic;

