#include "ExerciseBot.h"
#include <iostream>

ExerciseBot::ExerciseBot(int x, int y)
	:Enemy(x, y, 32, 32), texture(nullptr), frameindex(0)
{
	frames[0] = {0,0,32,39};
	frames[1] = {34,0,30,39};
	frames[2] = {0,39,30,39};
}

ExerciseBot::~ExerciseBot()
{

}

bool ExerciseBot::Init(SDL_Renderer* renderer)
{
	TextureLoader TextureLoader;
	texture = TextureLoader.LoadPNGTexture("assets/bot.png", renderer);
	
	if (!texture)
	{
		std::cerr << "Failed to Load bot.png Texture" << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void ExerciseBot::Update(Uint32 currentTIme)
{
	/*if (currentTIme % 1000 < 500)
	{
		frameindex = 1;
	}
	else
	{
		frameindex = 2;
	}*/
	if (gotHit && currentTIme - lastHitTime >= hitDuration)
	{
		gotHit = false;
	}
	
}

void ExerciseBot::Render(SDL_Renderer* Renderer)
{
	frameindex = gotHit ? 2 : 0;
	SDL_RenderCopy(Renderer, texture, &frames[frameindex], &dstRect);
}

void ExerciseBot::CleanUp()
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}
}

void ExerciseBot::OnHit()
{
	gotHit = true;
	lastHitTime = SDL_GetTicks();
}
