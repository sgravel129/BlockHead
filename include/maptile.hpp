#pragma once

#include "util.hpp"
#include "sprite.hpp"

class MapTile
{
private:
    Sprite *_sprite;
    bool _hasCollision;
    Point _location, rPos;

public:
    MapTile();
    MapTile(Graphics &graphics, const std::string &path, SDL_Rect src, float scale, bool hasCollision, Point location);

    ~MapTile();
    void update(Point delta);
    void draw(Graphics &graphics);

    // Accessors

    Point getPos() const;
    bool getCollision() const;
    SDL_Rect getDestRect();
};