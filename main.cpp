#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL.h>

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

//텍스처 로딩
SDL_Texture* LoadTexture(const char* path, SDL_Renderer* Renderer)
{
	SDL_Surface* tempSurface = SDL_LoadBMP(path);
	if (!tempSurface)
	{
		std::cerr << "Failed to Load BMP : " << SDL_GetError() << std::endl;
		return nullptr;
	}
	SDL_Texture* TempTexture = SDL_CreateTextureFromSurface(Renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	if (!TempTexture)
	{
		std::cerr << "Failed to Create Texture : " << SDL_GetError() << std::endl;
		return nullptr;
	}

	return TempTexture;
}

void InitTextureFrames(SDL_Rect Frames[], int count, int frameWidth, int frameHeght, int Columns, int Rows)
{

	for (int i = 0; i < count; i++)
	{
		int col = i % Columns;
		int row = i / Rows;
		Frames[i] = { col * frameWidth , row * frameHeght, frameWidth, frameHeght };
	}
}

void CleanUp(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* Textures[])
{
	for (int i = 0; i < 4; i++)
	{
		SDL_DestroyTexture(Textures[i]);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
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
	
	//텍스처로딩
	//Idle
	SDL_Texture* idleL = LoadTexture("assets/idle_left.bmp",Renderer);
	SDL_Texture* idleR = LoadTexture("assets/idle_right.bmp", Renderer);
	//Run
	SDL_Texture* runL = LoadTexture("assets/run_left.bmp", Renderer);
	SDL_Texture* runR = LoadTexture("assets/run_right.bmp", Renderer);
	//Jump
	SDL_Texture* JumpL = LoadTexture("assets/jump_left.bmp", Renderer);
	SDL_Texture* JumpR = LoadTexture("assets/jump_right.bmp", Renderer);

	SDL_Texture* Array_Textures[] = { idleL,idleR,runL,runR,JumpL,JumpR };
	
	for (SDL_Texture* Texture : Array_Textures)
	{
		if (!Texture)
		{
			return 1;
		}
	}

	//최초 위치
	SDL_Rect dstRect = { 304,224,32,32 };

	//프레임 초기화
	//기본적으로 3x3 텍스처 형태
	const int Frame_WIDTH = 32;
	const int Frame_HEIGHT = 32;
	const int IDLE_FRAME_COUNT = 9;
	const int RUN_FRAME_COUNT = 8;
	const int JUMP_FRAME_COUNT = 12;


	//텍스처 프레임 초기화
	SDL_Rect idleFrames[9];
	InitTextureFrames(idleFrames, IDLE_FRAME_COUNT,Frame_WIDTH,Frame_HEIGHT,3,3);
	SDL_Rect RunFrames[8];
	InitTextureFrames(RunFrames, RUN_FRAME_COUNT, Frame_WIDTH, Frame_HEIGHT,3,3);
	SDL_Rect JumpFrames[12];
	InitTextureFrames(JumpFrames, JUMP_FRAME_COUNT, Frame_WIDTH, Frame_HEIGHT, 3,4);
	
	//프레임 관련 변수 초기화
	int FrameIndex = 0;
	Uint32 lastFrameTime = 0;
	Uint32 frameInterval = 0;
	
	//상태 초기화
	float velocityY = 0.0f;
	bool isJumping = false;
	const int groundY = 224;
	float gravity = 0.5f;

	bool wasLeft = false;
	SDL_Texture* CurrentTexture = idleR;
	SDL_Rect* CurrentFrame = idleFrames;

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
		}

		bool isMoving = false;

		const Uint8* keyStates = SDL_GetKeyboardState(NULL);
		Uint32 Now = SDL_GetTicks();

		//Key에 따른 세팅 적용

		//이동
		if (keyStates[SDL_SCANCODE_A])
		{
			dstRect.x -= 4;
			wasLeft = true;
			isMoving = true;
		}
		else if (keyStates[SDL_SCANCODE_D])
		{
			dstRect.x += 4;
			wasLeft = false;
			isMoving = true;
		}

		//점프
		if (keyStates[SDL_SCANCODE_SPACE] && !isJumping)
		{
			std::cout << "Press SpaceBar" << std::endl;
			velocityY = -10.0f;
			FrameIndex = 0;
			isJumping = true;
		}

		//esc 누르면 종료
		else if (keyStates[SDL_SCANCODE_ESCAPE])
		{
			running = false;
		}

		//Jump
		if (isJumping)
		{
			//y좌표에 velocity를 더해서 적용하고
			dstRect.y += static_cast<int>(velocityY);
			//velocity에 gravity를 더해서 중력을 표현
			velocityY += gravity;

			//y좌표가 groundY와 같아지거나 커지면(더 아래로 내려가면) 초기화.
			if (dstRect.y >= groundY)
			{
				dstRect.y = groundY;
				velocityY = 0.0f;
				isJumping = false;

				std::cout << "Landed" << std::endl;

			}
		}

		//Run ? Idle ?

		if (isJumping)
		{
			isMoving = false;
			CurrentFrame = JumpFrames;
			CurrentTexture = wasLeft ? JumpL : JumpR;
			frameInterval = 200;

			if (velocityY < 0)
			{
				FrameIndex = (Now - lastFrameTime >= frameInterval) ? (FrameIndex + 1) % JUMP_FRAME_COUNT : FrameIndex;
			}
			else if (velocityY == 0)
			{
				FrameIndex = 6;
			}
			else if (velocityY > 0)
			{
				FrameIndex = (Now - lastFrameTime >= frameInterval) ? (7 + 1) % JUMP_FRAME_COUNT : FrameIndex;
			}
			else
			{
				FrameIndex = (Now - lastFrameTime >= frameInterval) ? (9 + 1) % JUMP_FRAME_COUNT : FrameIndex;
			}

		}

		else if (isMoving)
		{
			CurrentFrame = RunFrames;
			CurrentTexture = wasLeft ? runL : runR;
			frameInterval = 25;
			FrameIndex = (Now - lastFrameTime >= frameInterval) ? (FrameIndex + 1) % RUN_FRAME_COUNT : FrameIndex;
		}
		
		else
		{
			CurrentFrame = idleFrames;
			CurrentTexture = wasLeft ? idleL : idleR;
			frameInterval = 200;
			FrameIndex = (Now - lastFrameTime >= frameInterval) ? (FrameIndex + 1) % IDLE_FRAME_COUNT : FrameIndex;
		}
		
		
		

		if (Now - lastFrameTime >= frameInterval)
		{
			lastFrameTime = Now;
		}

		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, CurrentTexture, &CurrentFrame[FrameIndex], &dstRect);
		SDL_RenderPresent(Renderer);

		SDL_Delay(16);
	}

	CleanUp(Window, Renderer, Array_Textures);

	return 0;
}