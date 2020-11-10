#pragma once
#include "sprite.hpp"
#include <vector>

class Zombie
{
private:
    Sprite *_sprite;
    std::vector<SDL_Rect> _anims;
    int _currAnim;
    int _x, _y;
    int _angle; // for sprite info
    int _health;
    int _moveSpeed;

    // Path _pathToPlayer;  // tentative Path class, implemented with path finder algorithm

    // int _zombieType;    // might implement different zombie types

public:
    Zombie();
    Zombie(Graphics &graphics, const std::string &path, int w, int h, float scale);
    ~Zombie();
    void update(int, int);
    void draw(Graphics &graphics);
};