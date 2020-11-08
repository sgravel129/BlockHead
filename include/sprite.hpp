#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <string>

class Graphics;

class Sprite
{
public:
	Sprite();
	~Sprite();

    Sprite(Graphics &graphics, const std::string &path, SDL_Rect src, float scale);
	Sprite(Graphics &graphics, const std::string &path, int srcX, int srcY, int srcWidth, int srcHeight, float scale);


    /*
        Changes current _srcRect
	 */
    void change_src(SDL_Rect src);
	void change_scale(float scale);

    /*
		Renders the sprite to screen
	 */
	void draw(Graphics &graphics, int x, int y);

protected:

	SDL_Rect _srcRect;
	SDL_Texture *_spriteSheet;

private:
	float _scale;
};

#endif