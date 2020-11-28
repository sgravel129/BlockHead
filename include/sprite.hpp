#pragma once

#include "graphics.hpp"

#include <string>

struct Point
{
	int x;
	int y;
};

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

	/*
		Renders the sprite to screen
	 */
	void draw(Graphics &graphics, int x, int y);

private:
	float _scale;
	SDL_Rect _srcRect;
	SDL_Texture *_spriteSheet;
};