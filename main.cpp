#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL.h>

//SDL �ʱ�ȭ , ������ ���� , ������ ����
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

//�ؽ�ó �ε�
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
	//SDL �ʱ�ȭ
	SDL_Window* Window = nullptr;
	SDL_Renderer* Renderer = nullptr;
	if (!InitSDL(&Window, &Renderer))
	{
		return 1;
	}
	
	//�ؽ�ó�ε�
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

	//���� ��ġ
	SDL_Rect dstRect = { 304,224,32,32 };

	//������ �ʱ�ȭ
	//�⺻������ 3x3 �ؽ�ó ����
	const int Frame_WIDTH = 32;
	const int Frame_HEIGHT = 32;
	const int IDLE_FRAME_COUNT = 9;
	const int RUN_FRAME_COUNT = 8;
	const int JUMP_FRAME_COUNT = 12;


	//�ؽ�ó ������ �ʱ�ȭ
	SDL_Rect idleFrames[9];
	InitTextureFrames(idleFrames, IDLE_FRAME_COUNT,Frame_WIDTH,Frame_HEIGHT,3,3);
	SDL_Rect RunFrames[8];
	InitTextureFrames(RunFrames, RUN_FRAME_COUNT, Frame_WIDTH, Frame_HEIGHT,3,3);
	SDL_Rect JumpFrames[12];
	InitTextureFrames(JumpFrames, JUMP_FRAME_COUNT, Frame_WIDTH, Frame_HEIGHT, 3,4);
	
	//������ ���� ���� �ʱ�ȭ
	int FrameIndex = 0;
	Uint32 lastFrameTime = 0;
	Uint32 frameInterval = 0;
	
	//���� �ʱ�ȭ
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

		//Key�� ���� ���� ����

		//�̵�
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

		//����
		if (keyStates[SDL_SCANCODE_SPACE] && !isJumping)
		{
			std::cout << "Press SpaceBar" << std::endl;
			velocityY = -10.0f;
			FrameIndex = 0;
			isJumping = true;
		}

		//esc ������ ����
		else if (keyStates[SDL_SCANCODE_ESCAPE])
		{
			running = false;
		}

		//Jump
		if (isJumping)
		{
			//y��ǥ�� velocity�� ���ؼ� �����ϰ�
			dstRect.y += static_cast<int>(velocityY);
			//velocity�� gravity�� ���ؼ� �߷��� ǥ��
			velocityY += gravity;

			//y��ǥ�� groundY�� �������ų� Ŀ����(�� �Ʒ��� ��������) �ʱ�ȭ.
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