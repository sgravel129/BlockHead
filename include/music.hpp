#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "log.hpp"

bool initMusic();
bool loadMedia();
void close();

extern Mix_Music* introMusic;
extern Mix_Music* loopMusic;
