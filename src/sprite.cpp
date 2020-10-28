#include "sprite.hpp"
#include "graphics.hpp"
#include "util.hpp"
#include "log.hpp"

#include <iostream>
#include <algorithm>

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	// SDL_destRectroyTexture(_spriteSheet);
	Log::debug("~Sprite() | destroyed sprite");
}

// we will use a sprite sheet and then crop out the angle we want with a top
// left corner start of x and y values that will need to be determined
Sprite::Sprite(Graphics &graphics, const std::string &path, int x, int y, int w, int h, float scale)
{
	if(!Util::fileExists(path)){
		Log::error("Sprite() | Resource does not exist: " + path);
		return;
	}

	_scale = scale;

	_srcRect.x = x;
	_srcRect.y = y;
	_srcRect.w = w;
	_srcRect.h = h;

	_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(path));
	if (_spriteSheet == NULL)
	{
		Log::error("Sprite() | Failed at creating surface texture from path");
	}
}

Sprite::Sprite(Graphics &graphics, const std::string &path, int w, int h, float scale)
{
	new (this) Sprite(graphics, path, 0, 0, w, h, scale);
}

void Sprite::change_src(SDL_Rect src){
    _srcRect = src;
}

void Sprite::draw(Graphics &graphics, int x, int y)
{
	SDL_Rect destRect = {x, y, Util::scale(_srcRect.w, _scale), Util::scale(_srcRect.h, _scale)};
	graphics.blitSurface(_spriteSheet, &_srcRect, &destRect);
}