# pragma once
#include "sprite.hpp"
#include <vector>

class Map
{
private:
    Sprite _sprite;
    std::vector<SDL_Rect> _tiles;
    FILE _mapfile;
    // int _sizeX, _sizeY;
    // TODO: Create a struct of coords: (int, int)

public:
    Map(/* args */);
    Map(Graphics &graphics, const std::string &path,int w, int h, float scale);
    ~Map();
    void draw(Graphics &graphics);
};