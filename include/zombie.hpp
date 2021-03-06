#pragma once

#include "sprite.hpp"

#include <vector>

class Zombie
{
private:
    Sprite *sprite;
    std::vector<std::vector<SDL_Rect>> anims;
    int currAnim;
    Point rPos;
    Point pos, prevPos;
    int angle; // for sprite info
    int health;
    int moveSpeed;

    float _scale;
    int _w, _h;
    std::vector<int> _pathToPlayer;

    // int _zombieType;    // might implement different zombie types

public:
    Zombie();
    Zombie(Graphics &graphics, const std::string &path, int w, int h, float scale, Point playerP);
    ~Zombie();

    void update(Point delta, Point playerPos);
    void updateCamera(Point delta);
    void draw(Graphics &graphics);
    void setPath(const std::vector<int>&);
    Point getPos() const { return pos; }
    Point getRPos() const { return rPos; }
    std::vector<int> getPath() const { return _pathToPlayer; }

    Point getSize() const { return Point{ _w, _h }; }
    float getScale() const { return _scale; }
    Point posToTile(const Point&);
    SDL_Rect getDestRect();
 };


