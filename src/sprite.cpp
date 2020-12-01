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
bool operator==(const Point& LHS, const Point& RHS) { return (LHS.x == RHS.x && LHS.y == RHS.y); }
bool operator!=(const Point& LHS, const Point& RHS) { return !(LHS == RHS); }
std::string Point::to_string() {
	return "(" + std::to_string(this->x) + "," + std::to_string(this->y) + ")";
}

std::string to_string(SDL_Rect rect) {
	return "(" + std::to_string(rect.x) + "," + std::to_string(rect.y) +  "," +
		std::to_string(rect.w) + "," + std::to_string(rect.h) +")";
}

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	// Log::debug("~Sprite\t| Called");
	Log::destruct("Sprite\t| Destroy Texture");
	SDL_DestroyTexture(_spriteSheet);
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

	_scale = scale;
	_srcRect = src;

	_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(path));
	if (_spriteSheet == NULL)
	{
		Log::error("Sprite() | Failed at creating surface texture from path");
	}
}

void Sprite::change_src(SDL_Rect src)
{
	_srcRect = src;
}

Point Sprite::getDim()
{
	return Point{Util::scale(_srcRect.w, _scale), Util::scale(_srcRect.h, _scale)};
}

void Sprite::change_scale(float scale)
{
	_scale = scale;
}

void Sprite::draw(Graphics &graphics, int x, int y)
{
	SDL_Rect destRect = {x, y, Util::scale(_srcRect.w, _scale), Util::scale(_srcRect.h, _scale)};
	graphics.blitSurface(_spriteSheet, &_srcRect, &destRect);
}