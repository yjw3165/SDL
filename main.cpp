#define SDL_MAIN_HANDLED

#include <iostream>

//SDL
#include <SDL.h>
#include <SDL_image.h>

//Classes
#include "Player.h"
#include "TileMap.h"
#include "backgroundMap.h"
//SDL �ʱ�ȭ , ������ ���� , ������ ����
bool(InitSDL(SDL_Window** window, SDL_Renderer** renderer))
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Failed to Initialize SDL : " << SDL_GetError() << std::endl;
		return false;
	}

	*window = SDL_CreateWindow("SDL TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED );
	if(*window == nullptr)
	{
		std::cerr << "Failed to Create Window : " << SDL_GetError() << std::endl;
		return false;
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if(*renderer == nullptr)
	{
		std::cerr << "Failed to CreateRenderer : " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_RenderSetLogicalSize(*renderer, 1920, 1080);

	return true;
}

int main()
{
	//SDL �ʱ�ȭ
	SDL_Window* Window = nullptr;
	SDL_Renderer* Renderer = nullptr;
	if (!InitSDL(&Window, &Renderer))
	{
		return 1;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		std::cerr << "Failed to Initialize SDL_Image : " << SDL_GetError() << std::endl;
		return 1;
	}
	
	//backgroundMap Ŭ���� Init
	backgroundMap BG;
	if (!BG.init(Renderer))
	{
		std::cerr << "Failed to Init backgroundMap Class" << std::endl;
		return 1;
	}
	//Player Ŭ���� Init
	Player Player;
	if (!Player.Init(Renderer))
	{
		std::cerr << "Failed to Init Player Class" << std::endl;
		return 1;
	}

	//TileMap Ŭ���� Init
	TileMap TileMap;
	if (!TileMap.init(Renderer))
	{
		std::cerr << "Failed to Init TileMap Class" << std::endl;
		return 1;
	}

	const Uint8* keyStates = SDL_GetKeyboardState(NULL);
	bool running = true;
	SDL_Event event;

	//���� ����
	while (running)
	{
		//Ű �̺�Ʈ ����
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			if (keyStates[SDL_SCANCODE_ESCAPE])
			{
				running = false;
			}
		}

		//�Է� ����
		Player.HandleInput(keyStates);

		//�ִϸ��̼� / ������ ������Ʈ
		Player.Update(SDL_GetTicks());

		

		//������ �������
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);

		//ȭ�� �����
		SDL_RenderClear(Renderer);
		
		//backgroundMap ������
		BG.Render(Renderer);
		//TileMap ������
		TileMap.Render(Renderer);
		//�÷��̾� ������
		Player.Render(Renderer);

		SDL_RenderPresent(Renderer);

		//FPS���� 16ms = 60FPS
		SDL_Delay(16);
	}

	

	//������ ������ ��������
	//���� : SDL ����-> ������ ���� -> ������ ���� -> �ؽ�ó ����
	//���� : �ؽ�ó ���� -> ������ ���� -> ������ ���� -> SDL ����
	
	//BGMap ����
	BG.CleanUp();
	//TileMap ����
	TileMap.CleanUp();
	//�÷��̾ ���� �ؽ�ó ����
	Player.CleanUp();
	//������ ����
	SDL_DestroyRenderer(Renderer);
	//������ ����
	SDL_DestroyWindow(Window);
	//SDL_Image ����
	IMG_Quit();
	//SDL_����
	SDL_Quit();

	return 0;
}