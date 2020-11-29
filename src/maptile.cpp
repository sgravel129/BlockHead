#include "maptile.hpp"
#include "log.hpp"

// TODO: Support multiple lines for mapfiles. fix commas.

MapTile::MapTile() {}
MapTile::~MapTile()
{
    Log::destruct("MapTile\t| Destroy: " + std::to_string(_location.x) + " " + std::to_string(_location.y));
    delete _sprite;
}

// Accessors
Point MapTile::getPos() const { return _location; }
bool MapTile::getCollision() const { return _hasCollision; }

MapTile::MapTile(Graphics &graphics, const std::string &path, SDL_Rect src, float scale, bool hasCollision, Point location){
    _sprite = new Sprite(graphics, path, src, scale);
    _hasCollision = hasCollision;
    _location = location;
}
void MapTile::update(Point delta){
    _location = _location - delta;
}
void MapTile::draw(Graphics &graphics)
{
    _sprite->draw(graphics, _location.x, _location.y);
}