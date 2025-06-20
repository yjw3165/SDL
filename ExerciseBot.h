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

private:
	SDL_Texture* texture;
	SDL_Rect frames[3];
	int frameindex;
};

