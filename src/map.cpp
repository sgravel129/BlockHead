#include "map.hpp"
#include "log.hpp"

#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <boost/algorithm/string.hpp>

// TODO: Ignore srcRectStr for 0. Index starting at 1.

Map::Map()
{
}

Map::~Map()
{
    Log::destruct("Map\t| Destroy MapTiles:");
    for (auto &tile : _tiles)
    {
        tile->~MapTile();
    }
}

Map::Map(Point size)
{
    _size = size;
    Log::verbose("map | Loaded Map");
}

void Map::loadTextures(const std::string &spriteSheet, const std::string &spritesProps)
{
    if (!Util::fileExists(spriteSheet))
    {
        Log::error("map | Could not find spriteSheet: " + spriteSheet);
        throw std::runtime_error("Map file does not exist: " + spriteSheet);
    }
    if (!Util::fileExists(spritesProps))
    {
        Log::error("map | Could not find spriteProps: " + spritesProps);
        throw std::runtime_error("Map file does not exist: " + spritesProps);
    }
    _spriteSheet = spriteSheet;

    std::ifstream spritefile;
    spritefile.open(spritesProps, std::ios::in);
    int tile = 0;
    Point location = Point{0, 0};
    if (spritefile.is_open())
    { //checking whether the file is open
        std::string line = "";
        while (getline(spritefile, line))
        { //read data from file object and put it into string.
            std::vector<std::string> srcVec;
            boost::algorithm::split(srcVec, line, boost::is_any_of(", \t\n\0"),
                                    boost::token_compress_on);
            try
            {
                Log::debug("map", "Adding TileProps: " + line);
                _tileProps.push_back(SDL_Rect{  stoi(srcVec[0]),
                                                stoi(srcVec[1]),
                                                stoi(srcVec[2]),
                                                stoi(srcVec[3])});
            }
            catch (const std::exception &e)
            {
                Log::error("map | Could not create spriteProp: " + line);
            }
        }
    }
}

void Map::loadMapFile(Graphics &graphics, const std::string &mapfilePath)
{
    if (!Util::fileExists(mapfilePath))
    {
        Log::error("map | Could not find mapFile: " + mapfilePath);
        throw std::runtime_error("Map file does not exist: " + mapfilePath);
    }
    std::ifstream mapfile;
    mapfile.open(mapfilePath, std::ios::in);
    int tile = 0;
    Point location = Point{0, 0};
    if (mapfile.is_open())
    { //checking whether the file is open
        std::string line = "";
        while (getline(mapfile, line))
        { //read data from file object and put it into string.
            std::vector<std::string> srcVec;
            boost::algorithm::split(srcVec, line, boost::is_any_of(", \t\n\0"),
                                    boost::token_compress_on);
            for (auto &idxSrc : srcVec)
            {
                if (idxSrc.empty())
                {
                    continue; // ignore empty and null strings
                }
                if (stoi(idxSrc) <=0)
                {
                    location.x += 200;
                    continue;
                }

                try
                {
                    _tiles.push_back(
                        new MapTile(graphics,
                                    _spriteSheet,
                                    _tileProps[stoi(idxSrc)-1],
                                    5.0,
                                    false,
                                    location));

                    Log::debug("map", "Created MapTile: " + idxSrc +
                               " @ " + std::to_string(location.x) + " " +
                               std::to_string(location.y));
                }
                catch (const std::exception &e)
                {
                    // TODO: Handle undefined spriteSheet by not calling
                    // loadTexture first
                    Log::error("map | Could not create tile: " + idxSrc);
                }
                location.x += 200;
            }
            location.x = 0;
            location.y += 200;
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
