#define SDL_MAIN_HANDLED

#include <iostream>

//SDL
#include <SDL.h>
#include <SDL_image.h>

//Classes
#include "Player.h"
#include "TileMap.h"
#include "backgroundMap.h"
#include "Enemy.h"
#include "ExerciseBot.h"


//SDL 초기화 , 윈도우 생성 , 렌더러 생성
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
	//SDL 초기화
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
	
	//backgroundMap 클래스 Init
	backgroundMap BG;
	if (!BG.init(Renderer))
	{
		std::cerr << "Failed to Init backgroundMap Class" << std::endl;
		return 1;
	}
	//Player 클래스 Init
	Player Player;
	if (!Player.Init(Renderer))
	{
		std::cerr << "Failed to Init Player Class" << std::endl;
		return 1;
	}

	//ExerciseBot 클래스 Init
	Enemy* bot = new ExerciseBot(1200, 735);
	if (!bot->Init(Renderer))
	{
		std::cerr << "Failed to Init bot" << std::endl;
		return 1;
	}

	//TileMap 클래스 Init
	TileMap TileMap;
	if (!TileMap.init(Renderer))
	{
		std::cerr << "Failed to Init TileMap Class" << std::endl;
		return 1;
	}

	const Uint8* keyStates = SDL_GetKeyboardState(NULL);
	bool running = true;
	SDL_Event event;

	//메인 루프
	while (running)
	{
		//키 이벤트 제어
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

		// 1. 입력 제어
		Player.HandleInput(keyStates);


		// 2. 상태 업데이트
		// 2-1 플레이어 업데이트
		Player.Update(SDL_GetTicks());
		// 2-2 훈련용 봇 업데이트
		bot->Update(SDL_GetTicks());
		// 2-3 피격 판정 체크
		if (Player.isAttackFrame())
		{
			const SDL_Rect& hitbox = Player.GetAttackHitBox();
			if (bot->isHit(hitbox))
			{
				bot->OnHit();
			}
		}
		
		// 3. 렌더링
		//아래 순서대로 렌더링 함 = 레이어 순서
		//렌더링이 빨리 될 수록 뒤쪽으로 가고
		//렌더링이 늦게 될 수록 앞쪽으로 나온다.
		//그래서 지금은 플레이어가 가장 앞쪽으로 나와있음
		
		//바탕은 흰색으로
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
		//화면 지우기
		SDL_RenderClear(Renderer);
		
		//backgroundMap 렌더링
		BG.Render(Renderer);
		//TileMap 렌더링
		TileMap.Render(Renderer);

		//훈련용 봇 렌더링
		bot->Render(Renderer);
		//플레이어 렌더링
		Player.Render(Renderer);

		SDL_RenderPresent(Renderer);

		//FPS설정 16ms = 60FPS
		SDL_Delay(16);
	}

	

	//정리는 시작의 역순으로
	//시작 : SDL 생성-> 윈도우 생성 -> 렌더러 생성 -> 텍스처 생성
	//종료 : 텍스처 정리 -> 렌더러 종료 -> 윈도우 종료 -> SDL 종료
	
	//플레이어에 사용된 텍스처 정리
	Player.CleanUp();
	//Exercise bot 정리
	bot->CleanUp();
	//TileMap 정리
	TileMap.CleanUp();
	//BGMap 정리
	BG.CleanUp();
	
	//렌더러 정리
	SDL_DestroyRenderer(Renderer);
	//윈도우 정리
	SDL_DestroyWindow(Window);
	//SDL_Image 종료
	IMG_Quit();
	//SDL_종료
	SDL_Quit();

	return 0;
}