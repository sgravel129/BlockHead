#include "zombie.hpp"
#include "log.hpp"

Zombie::Zombie()
{
}

Zombie::~Zombie()
{
    Log::debug("~Zombie\t| Called");
    _sprite->~Sprite();
}

Zombie::Zombie(Graphics &graphics, const std::string &path,int w, int h, float scale)
{
    _x = 500;
    _y = 500;
    _angle = 0; // starting direction
    _moveSpeed = 1;

    _sprite = new Sprite(graphics, path, w, h, scale);
    for (int i = 0; i < 4; i++)
    {
        _anims.emplace_back(SDL_Rect{96, h*i, w, h});
    }
}

void Zombie::update(int target_x, int target_y){
    // Do path finding. Euclidian Distance
    const int DEADBAND = 5;
    if ((_y - target_y) > DEADBAND)
    {
        // need to move UP
        _y -= _moveSpeed;
        _angle = 3;
    } else if ((_y - target_y) < DEADBAND){
        // need to move DOWN
        _y += _moveSpeed;
        _angle = 0;
    }
    if ((_x - target_x) < DEADBAND){
        // need to move LEFT
        _x += _moveSpeed;
        _angle = 2;
    } else if ((_x - target_x) > DEADBAND)
    {
        // need to move RIGHT
        _x -= _moveSpeed;
        _angle = 1;
    }
}

void Zombie::draw(Graphics &graphics){
    _sprite->change_src(_anims[_angle]);
    _sprite->draw(graphics, _x, _y);
}
