#pragma once

#include "graphics.hpp"

#include <string>

struct Point
{
	int x;
	int y;

	Point operator -(const Point& b);
	Point operator +(const Point& b);
	std::string to_string();
};
std::string to_string(SDL_Rect);

bool operator==(const Point&, const Point&);
bool operator!=(const Point&, const Point&);

class Sprite
{
public:
	Sprite();
	~Sprite();

	Sprite(Graphics &graphics, const std::string &path, SDL_Rect src, float scale);

	/*
        Changes current _srcRect
	 */
	void change_src(SDL_Rect src);
	void change_scale(float scale);
	Point getDim();

	/*
		Renders the sprite to screen
	 */
	void draw(Graphics &graphics, int x, int y);
	

private:
	float _scale;
	SDL_Rect _srcRect;
	SDL_Texture *_spriteSheet;
};