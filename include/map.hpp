# pragma once

#include <vector>
#include <fstream>
#include <string>

#include "util.hpp"
#include "maptile.hpp"

class Map
{
private:
    std::vector<MapTile> _tiles;
    std::vector<SDL_Rect> _tileProps;
    Point _size;
    // TODO: Create a struct of coords: (int, int)

public:
    Map(/* args */);
    Map(Point size);
    ~Map();
    void loadTileProps(const std::string &propfile);
    void loadMapFile(Graphics &graphics, const std::string &mapfile, const std::string &spriteSheet);
    void draw(Graphics &graphics);
};