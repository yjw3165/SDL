#include "backgroundMap.h"
#include "TextureLoader.h"
#include <iostream>

bool backgroundMap::init(SDL_Renderer* Renderer)
{
	TextureLoader TextureLoader;
	//���� �ؽ�ó �ݺ� ������
	BGTexture = TextureLoader.LoadPNGTexture("assets/BG_Tile.png", Renderer);

	//ū �ؽ�ó ���� ������
	//BGTexture = TextureLoader.LoadTexture("assets/BG.bmp", Renderer);
	if (!BGTexture)
	{
		std::cerr << "Failed to Load Texture BG.png" << std::endl;
		return false;
	}

	//���� �ؽ�ó  �ݺ� ������
	TileClips[0] = { 0,0,128,128 }; // -> �ϼ���
	TileClips[1] = { 128, 0 ,128,128 }; // -> ���
	TileClips[2] = { 0,128,128,128 }; //�����

	//��� ���� 
	//[�ϼ��� / ��� / ���] ... �ݺ�

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (i == 5)
			{
				int patternIndex = j % 3;
				if (patternIndex == 0)
				{
					map[i][j] = 0;
				}
				else
				{
					map[i][j] = 1;
				}
			}
			else
			{
				map[i][j] = 2;
			}
		}
	}

	return true;
}

void backgroundMap::Render(SDL_Renderer* Renderer)
{
	//���� �ؽ�ó �ݺ� ������
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			int type = map[y][x];

			SDL_Rect dest = { x * 128, y * 128,128,128 };
			SDL_RenderCopy(Renderer, BGTexture, &TileClips[type], &dest);
		}
	}

	//ū �ؽ�ó ���� ������
	/*SDL_Rect dstRect = { 0,0,1920,1080 };
	SDL_RenderCopy(Renderer, BGTexture, nullptr, &dstRect);*/
}

void backgroundMap::CleanUp()
{
	SDL_DestroyTexture(BGTexture);
}
