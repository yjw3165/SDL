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

    SDL_FreeSurface(tempSurface); //�޸𸮿��� GPU�� ���� �� �ڿ� �޸𸮿� ���� �ִ� �� ����

    if (!tempTexture)
    {
        std::cerr << "Failed to Create Texture : " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return tempTexture;
}
