#include "collision.hpp"
bool collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB)
{
	if (
		recA.x + 32 >= recB.x &&
		recB.x + 32>= recA.x &&
		recA.y + 32 >= recB.y &&
		recB.y + 32 >= recA.y

		)
	{
		return true;
	}
	return false;
}
// check to see if tile is colidable 
bool Collision::AABB(const hasCollision& colA, const hasCollision& colb) {
	if (AABB(colA.collider, colB.collider)) {
		return true;
	}
	else {
		return false
	}
}