#pragma once
#include <vector>
// #include <SDL.h>

#include "sprite.hpp"
#include "input.hpp"

class Player
{
private:
    /* data */
    Sprite *_sprite;
    std::vector<SDL_Rect> _anims;
    int _currAnim;
    int _x, _y;
    int _angle;  // for the sprite sheet and shooting aim -> map to 6 cardinal points (make cardinal direction class)
    int _health; // will need a numerical value assignmnet within TBD
    int _moveSpeed;
    //int _weapon;      // will depend on the weapon class
    //int _weaponL;     // list of owned weapons, same as above ^^
    //int _powerUp;     // active powerUp, depends on powerUp class
public:
    Player();
    Player(Graphics &graphics, const std::string &path, int w, int h, float scale);
    ~Player();
    void update(Input input);
    void draw(Graphics &graphics);
    int getX() { return _x; }
    int getY() { return _y; }
};