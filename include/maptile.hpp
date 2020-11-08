# pragma once

#include "util.hpp"
#include "sprite.hpp"

class MapTile
{
private:
    Sprite _sprite;
    bool _hasCollision;
    Point _location;

public:
    MapTile(/* args */);
    MapTile(Graphics &graphics, const std::string &path, bool hasCollision, Point location);
    MapTile(Sprite sprite, bool hasCollision, Point location);
    ~MapTile();
    void draw(Graphics &graphics);
};