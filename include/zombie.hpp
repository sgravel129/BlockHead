#pragma once

#include "sprite.hpp"

#include <vector>

class Zombie
{
private:
    Sprite *sprite;
    std::vector<std::vector<SDL_Rect>> anims;
    int currAnim;
    Point pos;
    int angle; // for sprite info
    int health;
    int moveSpeed;

    // Path _pathToPlayer;  // tentative Path class, implemented with path finder algorithm

    // int _zombieType;    // might implement different zombie types

public:
    Zombie();
    Zombie(Graphics &graphics, const std::string &path, int w, int h, float scale);
    ~Zombie();
    void update(int, int);
    void draw(Graphics &graphics);
};