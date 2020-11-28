#include "sprite.hpp"
#include "graphics.hpp"
#include "util.hpp"
#include "log.hpp"

#include <iostream>
#include <algorithm>

Point Point::operator -(const Point& b) {
    return Point{this->x - b.x, this->y - b.y};
}
Point Point::operator +(const Point& b) {
	return Point{this->x + b.x, this->y + b.y};
}
std::string Point::to_string() {
	return "(" + std::to_string(this->x) + "," + std::to_string(this->y) + ")";
}

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	Log::destruct("Sprite\t| Destroy Texture");
	SDL_DestroyTexture(spriteSheet);
}

// we will use a sprite sheet and then crop out the angle we want with a top
// left corner start of x and y values that will need to be determined

Sprite::Sprite(Graphics &graphics, const std::string &path, SDL_Rect src, float scale)
{
	if (!Util::fileExists(path))
	{
		Log::error("Sprite() | Resource does not exist: " + path);
		return;
	}

	scale = scale;
	srcRect = src;

	spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(path));
	if (spriteSheet == NULL)
	{
		Log::error("Sprite() | Failed at creating surface texture from path");
	}
}

void Sprite::change_src(SDL_Rect src)
{
	srcRect = src;
}
void Sprite::change_scale(float scale)
{
	scale = scale;
}

void Sprite::draw(Graphics &graphics, int x, int y)
{
	SDL_Rect destRect = {x, y, Util::scale(srcRect.w, scale), Util::scale(srcRect.h, scale)};
	graphics.blitSurface(spriteSheet, &srcRect, &destRect);
}