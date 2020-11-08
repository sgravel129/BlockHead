#include "maptile.hpp"


MapTile::MapTile(){}
MapTile::~MapTile(){}
// MapTile::MapTile(Graphics &graphics, const std::string &path, Point location){

// }
MapTile::MapTile(Sprite sprite, bool hasCollision, Point location){
    _sprite = sprite;
    _hasCollision = hasCollision;
    _location = location;
}
void MapTile::draw(Graphics &graphics){
    _sprite.draw(graphics, _location.x, _location.y);
}