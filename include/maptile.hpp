#pragma once

#include "util.hpp"
#include "sprite.hpp"

class MapTile
{
private:
    Sprite *_sprite;
    bool _hasCollision;
    Point _location;

public:
    MapTile();
    MapTile(Graphics &graphics, const std::string &path, SDL_Rect src, float scale, bool hasCollision, Point location);

    ~MapTile();
    void draw(Graphics &graphics);
};