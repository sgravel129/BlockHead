#include "zombie.hpp"
#include "log.hpp"
#include "animation.hpp"

// TODO: Migrate to point system.
const int MOVE_ANIMS = 3;
const int NUM_DIR = 4;

Zombie::Zombie()
{
}

Zombie::~Zombie()
{
    Log::destruct("Zombie\t| Called");
    _sprite->~Sprite();
}

Zombie::Zombie(Graphics &graphics, const std::string &path, int w, int h, float scale)
{
    _x = 500;
    _y = 500;
    _angle = 0; // starting direction
    _moveSpeed = 1;

    _sprite = new Sprite(graphics, path, SDL_Rect{0, 0, w, h}, scale);
    _anims.resize( NUM_DIR, std::vector<SDL_Rect>( MOVE_ANIMS ) );

    for (int i = 0; i < NUM_DIR; i++)
    {
        for(int j = 0; j < MOVE_ANIMS; j++){
            _anims[i][j] = SDL_Rect{w*j+96, h*i, w, h};
        }
    }
}
void Zombie::update(int target_x, int target_y)
{
    // Do path finding. Euclidian Distance
    const int DEADBAND = 5;
    if ((_y - target_y) > DEADBAND)
    {
        // need to move UP
        _y -= _moveSpeed;
        _angle = 3;
    }
    else if ((_y - target_y) < DEADBAND)
    {
        // need to move DOWN
        _y += _moveSpeed;
        _angle = 0;
    }
    if ((_x - target_x) < DEADBAND)
    {
        // need to move LEFT
        _x += _moveSpeed;
        _angle = 2;
    }
    else if ((_x - target_x) > DEADBAND)
    {
        // need to move RIGHT
        _x -= _moveSpeed;
        _angle = 1;
    }


    if (Animation::getTicks() % int(20 / _moveSpeed) == 0) {
        _currAnim = (_currAnim + 1) % MOVE_ANIMS;
    }
}

void Zombie::draw(Graphics &graphics)
{
    _sprite->change_src(_anims[_angle][_currAnim]);
    _sprite->draw(graphics, _x, _y);
}