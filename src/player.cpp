#include "player.hpp"
#include "log.hpp"
Player::Player()
{
}

Player::Player(Graphics &graphics, const std::string &path,int w, int h, float scale)
{
    _x = 200;
    _y = 200;
    _moveSpeed = 2;
    _angle = 0; // starting direction
    _sprite = new Sprite(graphics, path, 0, 0, w, h, scale);
    for (int i = 0; i < 4; i++)
    {
        _anims.emplace_back(SDL_Rect{0, h*i, w, h});
    }
}

Player::~Player()
{
    Log::debug("~Player\t| Called");
    _sprite->~Sprite();
}


void Player::update(Input input){
    if(input.isKeyHeld(SDL_SCANCODE_W)){
		_y -= _moveSpeed;
        _angle = 3;
    }
	if(input.isKeyHeld(SDL_SCANCODE_S)){
		_y += _moveSpeed;
        _angle = 0;
    }
	if(input.isKeyHeld(SDL_SCANCODE_D)){
		_x += _moveSpeed;
        _angle = 2;
    }
	if(input.isKeyHeld(SDL_SCANCODE_A)){
		_x -= _moveSpeed;
        _angle = 1;
    }
}
void Player::draw(Graphics &graphics){
    _sprite->change_src(_anims[_angle]);
    _sprite->draw(graphics, _x, _y);
}

