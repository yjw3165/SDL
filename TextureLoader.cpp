#include "TextureLoader.h"
#include <iostream>

SDL_Texture* TextureLoader::LoadTexture(const char* path, SDL_Renderer* Renderer)
{
    SDL_Surface* tempSurface = SDL_LoadBMP(path);
    if (!tempSurface)
    {
        std::cerr << "Failed to Load BMP : " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(Renderer, tempSurface);

    SDL_FreeSurface(tempSurface); //메모리에서 GPU로 복사 한 뒤에 메모리에 남아 있는 건 해제

    if (!tempTexture)
    {
        std::cerr << "Failed to Create Texture : " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return tempTexture;
}
