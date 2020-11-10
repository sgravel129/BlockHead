#pragma once

#include "util.hpp"
#include "maptile.hpp"

#include <vector>
#include <fstream>
#include <string>

class Map
{
private:
    std::vector<MapTile *> _tiles;
    std::vector<SDL_Rect> _tileProps;
    Point _size;

public:
    Map();
    Map(Point size);
    ~Map();
    void loadTileProps(const std::string &propfile);
    void loadMapFile(Graphics &graphics, const std::string &mapfile, const std::string &spriteSheet);
    void draw(Graphics &graphics);
};