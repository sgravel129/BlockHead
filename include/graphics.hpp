#pragma once

#include "color.hpp"

#include <map>
#include <string>

#if defined(_WIN32)
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

class Graphics
{
public:
	Graphics(const char *windowTitle, int screenWidth, int screenHeight);
	~Graphics();

	/*
		Loads image and stores it as texture in GPU
	 */
	SDL_Surface *loadImage(const std::string &path);

	/*
		Place na image in renderer
	 */
	void blitSurface(SDL_Texture *source, SDL_Rect *sourceRectagle, SDL_Rect *destinationRectangle);

	/*
		Renders renderer to screen
	 */
	void flip();

	/*
		Fills renderer background with set background color
	 */
	void fillBackground();

	SDL_Renderer *getRenderer() const;
	void setRenderColor(Color color);

private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;

	std::map<std::string, SDL_Surface *> _spriteSheets;
};