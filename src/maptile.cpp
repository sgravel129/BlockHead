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
Point MapTile::getPos() const { return _location; }
bool MapTile::getCollision() const { return _hasCollision; }

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


// Only putting collision on lower part of game objects (will look more natural given the "top down" view)
SDL_Rect MapTile::getDestRect(){
    Point dim = _sprite->getDim();
    double x = static_cast<double>(rPos.x) + static_cast<double>(dim.x) * 0.2;
    double y = static_cast<double>(rPos.y) + static_cast<double>(dim.y) * 0.5;
    double w = static_cast<double>(dim.x) * 0.6; double h = static_cast<double>(dim.y) * 0.3;
    return SDL_Rect{int(x), int(y), int(w), int(h)};
}