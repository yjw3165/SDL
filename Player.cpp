#include "Player.h"
#include "TextureLoader.h"
Player::Player()
	:idleTexture_R(nullptr), idleTexture_L(nullptr), runTexture_R(nullptr), runTexture_L(nullptr),
	jumpTexture_R(nullptr), jumpTexture_L(nullptr),
	dstRect{ 304,224,64,64 }, velocityY(0.0f), isJumping(false), wasLeft(false),
	groundY(224), currentState(State::Idle), 
	frameIndex(0), lastFrameTime(0),frameInterval(0)
{
}

bool Player::Init(SDL_Renderer* Renderer)
{
	TextureLoader TextureLoader;
	idleTexture_R = TextureLoader.LoadTexture("assets/idle_right.bmp", Renderer);
	idleTexture_L = TextureLoader.LoadTexture("assets/idle_left.bmp", Renderer);
	runTexture_R = TextureLoader.LoadTexture("assets/run_right.bmp", Renderer);
	runTexture_L = TextureLoader.LoadTexture("assets/run_left.bmp", Renderer);
	jumpTexture_R = TextureLoader.LoadTexture("assets/jump_right.bmp", Renderer);
	jumpTexture_L = TextureLoader.LoadTexture("assets/jump_left.bmp", Renderer);
	
	Array_Textures = { 
		idleTexture_L,idleTexture_R,
		runTexture_L,runTexture_R,
		jumpTexture_L,jumpTexture_R 
	};

	for (int i = 0; i < Array_Textures.size(); i++)
	{
		if (Array_Textures[i] == nullptr)
		{
			return false;
		}
	}

	//InitFrames 함수
	//아직 전역에서 사용되지 않을 뿐 더러, Init에서만 사용하겠다는 의미.
	auto InitFrames = [](SDL_Rect frames[], int count, int w, int h, int cols, int rows)
		{
			for (int i = 0; i < count; i++)
			{
				int col = i % cols;
				int row = i / rows;
				frames[i] = { col * w, row * h, w,h };
			}
		};

	InitFrames(idleFrames, 9, 32, 32, 3, 3);
	InitFrames(runFrames, 8, 32, 32, 3, 3);
	InitFrames(jumpFrames, 12, 32, 32, 3, 4);

	return true;
}

void Player::CleanUp()
{
	//로딩한 텍스처 삭제.
	for (SDL_Texture* Texture : Array_Textures)
	{
		SDL_DestroyTexture(Texture);
	}
}

void Player::HandleInput(const Uint8* keyStates)
{
	bool moving = false;
	if (keyStates[SDL_SCANCODE_A])
	{
		dstRect.x -= 6;
		wasLeft = true;
		moving = true;
	}
	else if (keyStates[SDL_SCANCODE_D])
	{
		dstRect.x += 6;
		wasLeft = false;
		moving = true;
	}

	if (keyStates[SDL_SCANCODE_SPACE] && !isJumping)
	{
		isJumping = true;
		velocityY = -15.0f;
		frameIndex = 0;
	}

	if (isJumping)
	{
		currentState = State::Jump;
	}
	else if (moving)
	{
		currentState = State::Run;
	}
	else
	{
		currentState = State::Idle;
	}
}

void Player::Update(Uint32 CurrentTime)
{
	UpdatePhysics();
	UpdateAnimation(CurrentTime);
	WrapAroundScreen(640);
}

void Player::Render(SDL_Renderer* Renderer)
{
	SDL_Rect* currentFrames = nullptr;
	SDL_Texture* texture = nullptr;

	switch (currentState)
	{
	case State::Idle:
		currentFrames = idleFrames;
		texture = wasLeft ? idleTexture_L : idleTexture_R;
		break;

	case State::Run:
		currentFrames = runFrames;
		texture = wasLeft ? runTexture_L : runTexture_R;
		break;

	case State::Jump:
		currentFrames = jumpFrames;
		texture = wasLeft ? jumpTexture_L : jumpTexture_R;
		break;

	}

	if (currentFrames && texture)
	{
		SDL_RenderCopy(Renderer, texture, &currentFrames[frameIndex], &dstRect);
	}
}

void Player::UpdateAnimation(Uint32 currentTime)
{
	SDL_Rect* frames = nullptr;
	int frameCount = 0;

	switch (currentState)
	{
	case State::Idle:
		frames = idleFrames;
		frameCount = 9;
		frameInterval = 200;
		break;

	case State::Run:
		frames = runFrames;
		frameCount = 8;
		frameInterval = 50;
		break;
	case State::Jump:
		frames = jumpFrames;
		frameCount = 12;
		frameInterval = 150;
		break;
	}

	if (isJumping)
	{
		if (velocityY < 0)
		{
			if (currentTime - lastFrameTime >= frameInterval)
			{
				frameIndex = (frameIndex + 1) % frameCount;
				lastFrameTime = currentTime;
			}
		}
		else if (velocityY == 0)
		{
			if (currentTime - lastFrameTime >= frameInterval)
			{
				frameIndex = (5 + 1) % frameCount;
				lastFrameTime = currentTime;
			}
		}
		else if (velocityY >= 0)
		{
			if (currentTime - lastFrameTime >= frameInterval)
			{
				frameIndex = (7 + 1) % frameCount;
				lastFrameTime = currentTime;
			}
		}
		
	}
	else
	{
		if (currentTime - lastFrameTime >= frameInterval)
		{
			frameIndex = (frameIndex + 1) % frameCount;
			lastFrameTime = currentTime;
		}
	}
}

void Player::UpdatePhysics()
{
	if (isJumping)
	{
		dstRect.y += static_cast<int>(velocityY);
		velocityY += 0.5f;

		if (dstRect.y >= groundY)
		{
			isJumping = false;
			dstRect.y = groundY;
			velocityY = 0;
		}
	}
}

void Player::WrapAroundScreen(int screenWidth)
{
	if (dstRect.x + dstRect.w < 0)
	{
		dstRect.x = screenWidth;
	}
	else if (dstRect.x > screenWidth)
	{
		dstRect.x = -dstRect.w;
	}
}
