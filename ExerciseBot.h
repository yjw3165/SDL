#pragma once
#include "Enemy.h"
#include "TextureLoader.h"

class ExerciseBot : public Enemy
{
public:
	ExerciseBot( int x, int y );
	~ExerciseBot();

	bool Init(SDL_Renderer* renderer) override;
	void Update(Uint32 currentTIme) override;
	void Render(SDL_Renderer* Renderer) override;
	void CleanUp() override;
	void OnHit() override;

private:
	SDL_Texture* texture = nullptr;
	SDL_Texture* hitTexture = nullptr;
	SDL_Rect frames[3];
	int frameindex;

	bool gotHit = false;
	Uint32 lastHitTime = 0;
	static constexpr Uint32 hitDuration = 200;
};


