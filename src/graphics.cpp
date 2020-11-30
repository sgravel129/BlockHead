#include "graphics.hpp"
#include "log.hpp"

#if defined(_WIN32)
	#include <SDL_image.h>
#else
	#include <SDL2/SDL_image.h>
#endif

Graphics::Graphics(const char *windowTitle, int screenWidth, int screenHeight)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Log::error("Graphics | SDL Subsystems failed to init: " + std::string(SDL_GetError()));
		throw std::runtime_error("FATAL ERROR: SDL Subsystem failed to initiate");
	}

	_window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	// SDL_SetWindowTitle(_window, windowTitle);
	_renderer = SDL_CreateRenderer(_window, -1, 0);
}

Graphics::~Graphics()
{
	Log::destruct("Graphics\t| Called");
	Log::destruct("Graphics\t| Destroy Surfaces:");
	for (auto &surf : _spriteSheets)
	{
		Log::destruct("Graphics\t| " + surf.first);
		SDL_FreeSurface(surf.second);
	}
	Log::destruct("Graphics\t| Destroy Renderer");
	SDL_DestroyRenderer(_renderer);
	Log::destruct("Graphics\t| Destroy Window");
	SDL_DestroyWindow(_window);
	Log::debug("~Graphics\t| Shutdown SDL Subsystem");
	SDL_Quit();
}

SDL_Surface *Graphics::loadImage(const std::string &path)
{
	if (!_spriteSheets.count(path))
	{
		_spriteSheets[path] = IMG_Load(path.c_str());
	}
	return _spriteSheets[path];
}

void Graphics::blitSurface(SDL_Texture *texture, SDL_Rect *source, SDL_Rect *destination)
{
	SDL_RenderCopy(_renderer, texture, source, destination);
}

void Graphics::flip()
{
	SDL_RenderPresent(_renderer);
}

void Graphics::fillBackground()
{
	SDL_RenderClear(_renderer);
}

SDL_Renderer *Graphics::getRenderer() const
{
	return _renderer;
}

void Graphics::setRenderColor(Color color)
{
	SDL_SetRenderDrawColor(
		_renderer,
		color.getRed(),
		color.getGreen(),
		color.getBlue(),
		color.getAlpha());
}