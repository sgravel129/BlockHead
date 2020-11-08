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
            // construct SDL_Rect
            std::vector<std::string> vec;
            boost::algorithm::split(vec, line, boost::is_any_of(","));
            for (auto str : vec) // access by reference to avoid copying
            {
                location.x += 200;
                // Log::verbose(str);
                Log::debug(std::to_string(_tileProps[stoi(str)].w));
                _tiles.push_back( MapTile(Sprite(graphics, spriteSheet, _tileProps[stoi(str)], 5.0), false, location) );
            }
            location.y += 30;
        }
    }
}

void Map::draw(Graphics &graphics)
{
    for (auto tile : _tiles) // access by reference to avoid copying
    {
        tile.draw(graphics);
    }
}

