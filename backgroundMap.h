#pragma once
#include <SDL.h>
#include <vector>

class backgroundMap
{
public :
	bool init(SDL_Renderer* Renderer);
	void Render(SDL_Renderer* Renderer);
	void CleanUp();
private:
	SDL_Texture* BGTexture = nullptr;
	SDL_Rect TileClips[3];

	static const int MAP_WIDTH = 15;
	static const int MAP_HEIGHT = 9;

	int map[MAP_HEIGHT][MAP_WIDTH];
};

