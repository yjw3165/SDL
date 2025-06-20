#pragma once
#include <SDL.h>

class Enemy
{

public:
	Enemy(int x, int y, int w, int h);
	virtual ~Enemy() = default;

	virtual bool Init(SDL_Renderer* renderer) = 0;
	virtual void Update(Uint32 currentTime) = 0;
	virtual void Render(SDL_Renderer* renderer) = 0;

	SDL_Rect GetRect() const { return dstRect; };
	bool isHit(const SDL_Rect& attackHitBox) const;

protected:
	SDL_Rect dstRect;
	int hp;
};

