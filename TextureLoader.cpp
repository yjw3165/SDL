#include "TextureLoader.h"
#include <iostream>
#include <SDL_image.h>
SDL_Texture* TextureLoader::LoadTexture(const char* path, SDL_Renderer* Renderer)
{
    //path에서 어셋 가져와서 메모리에 올림.
    SDL_Surface* tempSurface = SDL_LoadBMP(path);
    if (!tempSurface)
    {
        std::cerr << "Failed to Load BMP : " << SDL_GetError() << std::endl;
        return nullptr;
    }

    //메모리에 올라와있는 surface를 gpu에 넘김
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(Renderer, tempSurface);

    //GPU에 이미 복사 해놨기 때문에 더이상 필요 없음. 해제
    SDL_FreeSurface(tempSurface);

    if (!tempTexture)
    {
        std::cerr << "Failed to Create Texture : " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return tempTexture;
}

SDL_Texture* TextureLoader::LoadPNGTexture(const char* path, SDL_Renderer* Renderer)
{
    SDL_Surface* tempSurface = IMG_Load(path);
    if (!tempSurface)
    {
        std::cerr << "Failed to Load PNG : " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(Renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!tempTexture)
    {
        std::cerr << "Failed to Load Texture : " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return tempTexture;
}
