#pragma once

#include "sprite.hpp"

#include <vector>

class Zombie
{
private:
    Sprite *sprite;
    std::vector<std::vector<SDL_Rect>> anims;
    int currAnim;
    Point renderPos;
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
    void draw(Graphics &graphics);
    void setPath(const std::vector<int>&);
    Point getPos() const { return pos; }
    Point getRenderPos() const { return renderPos; }
    std::vector<int> getPath() const { return _pathToPlayer; }

    Point getSize() const { return Point{ _w, _h }; }
    float getScale() const { return _scale; }
    Point posToTile(const Point&);
    };

Point mapToPos(const Point&, const Point&, const float);