# pragma once

#include <vector>
#include <fstream>
#include <string>

#include "util.hpp"
#include "maptile.hpp"

class Map
{
private:
    std::vector<MapTile*> _tiles;
    std::vector<SDL_Rect> _tileProps;
    std::string _spriteSheet;
    Point _size;

public:
    Map(/* args */);
    Map(Point size);
    ~Map();
    void loadTextures(const std::string &spriteSheet, const std::string &spritesProps);
    void loadMapFile(Graphics &graphics, const std::string &mapfile);
    void draw(Graphics &graphics);
};