#pragma once

#include "util.hpp"
#include "maptile.hpp"

#include <vector>
#include <fstream>
#include <string>

//TODO add accessors;

class Map
{
private:
    std::vector<MapTile *> _tiles;
    std::vector<SDL_Rect> _tileProps;
    std::string _spriteSheet;
    Point _size;

public:
    Map();
    Map(Point size);
    ~Map();
    void loadTextures(const std::string &spriteSheet, const std::string &spritesProps);
    void loadMapFile(Graphics &graphics, const std::string &mapfile);
    void update(Point delta);
    void draw(Graphics &graphics);
};
