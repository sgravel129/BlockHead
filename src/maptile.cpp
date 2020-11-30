#include "maptile.hpp"
#include "log.hpp"

// TODO: Support multiple lines for mapfiles

MapTile::MapTile() {}
MapTile::~MapTile()
{
    Log::destruct("MapTile\t| Destroy: " + std::to_string(_location.x) + " " + std::to_string(_location.y));
    delete _sprite;
}

// Accessors
Point MapTile::getPos() { return _location; }
bool MapTile::getCollision() { return _hasCollision; }

MapTile::MapTile(Graphics &graphics, const std::string &path, SDL_Rect src, float scale, bool hasCollision, Point location){
    _sprite = new Sprite(graphics, path, src, scale);
    _hasCollision = hasCollision;
    _location = location;
    rPos = location;
}
void MapTile::update(Point delta){
    rPos = rPos - delta;
}
void MapTile::draw(Graphics &graphics)
{
    _sprite->draw(graphics, rPos.x, rPos.y);
}

SDL_Rect MapTile::getDestRect(){
    Point dim = _sprite->getDim();
    return SDL_Rect{rPos.x, rPos.y, dim.x, dim.y};
}