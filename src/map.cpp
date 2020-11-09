#include "map.hpp"

#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "log.hpp"

Map::Map()
{
}

Map::~Map()
{
    Log::debug("~Map\t| Destroy MapTiles:");
    for (auto &tile : _tiles)
    {
        tile->~MapTile();
    }
}

Map::Map(Point size)
{
    _size = size;
    _tileProps.push_back( SDL_Rect{0,0,15,15} );
    _tileProps.push_back( SDL_Rect{15,15,31,31} );
    Log::verbose("map | Loaded Map");
}

void Map::loadMapFile(Graphics &graphics, const std::string &mapfilePath, const std::string &spriteSheet)
{
    if (!Util::fileExists(mapfilePath)){
        throw std::runtime_error("Map file does not exist: " + mapfilePath);
    }
    std::ifstream mapfile;
    mapfile.open(mapfilePath, std::ios::in);
    int tile = 0;
    Point location = Point{0, 0};
    if (mapfile.is_open()){   //checking whether the file is open
        std::string line = "";
        while(getline(mapfile, line)){  //read data from file object and put it into string.
            std::vector<std::string> srcVec;
            boost::algorithm::split(srcVec, line, boost::is_any_of(","));
            for (auto &idxSrc : srcVec)
            {
                location.x += 200;
                Log::debug("map | Created MapTile: " + idxSrc + " @ " + std::to_string(location.x) + " " + std::to_string(location.y));
                _tiles.push_back( new MapTile(graphics, spriteSheet, _tileProps[stoi(idxSrc)], 5.0, false, location) );
            }
            location.y += 30;
        }
    }
}

void Map::draw(Graphics &graphics)
{
    for (auto &tile : _tiles)
    {
        tile->draw(graphics);
    }
}

