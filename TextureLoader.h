#pragma once
#include <SDL.h>

class TextureLoader
{
public:
	SDL_Texture* LoadTexture(const char* path, SDL_Renderer* Renderer);
	SDL_Texture* LoadPNGTexture(const char* path, SDL_Renderer* Renderer);
};

