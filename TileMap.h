#pragma once
#include <SDL.h>
#include <vector>

class TileMap
{
public: 
	bool init(SDL_Renderer* Renderer);
	void Render(SDL_Renderer* Renderer);
	void CleanUp();

private:
	SDL_Texture* tileTexture;
	SDL_Rect TileClips[2];

	static const int TILE_SIZE = 32;
	static const int MAP_WIDTH = 60;
	static const int MAP_HEIGHT = 34;

	int map[MAP_HEIGHT][MAP_WIDTH];

};

