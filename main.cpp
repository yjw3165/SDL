#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL.h>
#include "Player.h"

//SDL 초기화 , 윈도우 생성 , 렌더러 생성
bool(InitSDL(SDL_Window** window, SDL_Renderer** renderer))
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Failed to Initialize SDL : " << SDL_GetError() << std::endl;
		return false;
	}

	*window = SDL_CreateWindow("SDL TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

	SDL_RenderSetLogicalSize(*renderer, 640, 480);

	return true;
}

int main()
{
	//SDL 초기화
	SDL_Window* Window = nullptr;
	SDL_Renderer* Renderer = nullptr;
	if (!InitSDL(&Window, &Renderer))
	{
		return 1;
	}
	
	//Player 클래스 Init
	Player Player;
	if (!Player.Init(Renderer))
	{
		std::cerr << "Failed to Init Player" << std::endl;
		return 1;
	}

	const Uint8* keyStates = SDL_GetKeyboardState(NULL);
	bool running = true;
	SDL_Event event;
	while (running)
	{

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

		Player.HandleInput(keyStates);
		Player.Update(SDL_GetTicks());
		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
		SDL_RenderClear(Renderer);
		Player.Render(Renderer);
		SDL_RenderPresent(Renderer);

		SDL_Delay(16);
	}

	Player.CleanUp();
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();

	return 0;
}