#pragma once

#include <vector>

#include "sprite.hpp"
#include "input.hpp"

class Player
{
private:
    /* data */
    Sprite *sprite;
    std::vector<std::vector<SDL_Rect>> anims;
    int currAnim;
    Point pos, prevPos;
    int angle;  // for the sprite sheet and shooting aim -> map to 6 cardinal points (make cardinal direction class)
    int health; // will need a numerical value assignmnet within TBD
    int moveSpeed;
    //int _weapon;      // will depend on the weapon class
    //int _weaponL;     // list of owned weapons, same as above ^^
    //int _powerUp;     // active powerUp, depends on powerUp class
public:
    Player();
    Player(Graphics &graphics, const std::string &path, int w, int h, float scale);
    ~Player();
    void update(Input input);
    void draw(Graphics &graphics);
    int getX() { return pos.x; }
    int getY() { return pos.y; }
    Point getDeltaPos() { return pos - prevPos; }
    std::vector<SDL_Rect> getDestRects();
};