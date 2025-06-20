#include "Enemy.h"

Enemy::Enemy(int x, int y, int w, int h)
	:dstRect{ x,y,w,h },hp(100)
{
}

bool Enemy::isHit(const SDL_Rect& attackHitBox) const
{
	return SDL_HasIntersection(&dstRect, &attackHitBox);
}
