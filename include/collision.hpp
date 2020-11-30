#pragma once


class collision
{
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const hasCollision& colA, const hasCollision& colb)
};