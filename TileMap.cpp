#include "TileMap.h"
#include "TextureLoader.h"
#include <iostream>

bool TileMap::init(SDL_Renderer* Renderer)
{
    TextureLoader TextureLoader;
    tileTexture = TextureLoader.LoadTexture("assets/tile_test.bmp", Renderer);
    
    if (!tileTexture)
    {
        std::cerr << "Failed to Load TileTexture" << std::endl;
        return false;
    }

    //타일맵에서 어떤거 쓸지 결정
    TileClips[0] = { 32,32,TILE_SIZE,TILE_SIZE }; //-> 빈 타일
    TileClips[1] = { 32,0,TILE_SIZE,TILE_SIZE }; //-> 바닥 타일

    //9번째 줄은 바닥
    //나머지는 빈 타일
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            map[i][j] = (i == 24 ? 1 : 0);
        }
    }
    return true;
}

void TileMap::Render(SDL_Renderer* Renderer)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            int type = map[y][x];

            SDL_Rect dest = { x * TILE_SIZE, y * TILE_SIZE,32,32};
            SDL_RenderCopy(Renderer, tileTexture, &TileClips[type], &dest);
        }
    }
}

void TileMap::CleanUp()
{
    SDL_DestroyTexture(tileTexture);
}
