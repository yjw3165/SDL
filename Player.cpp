#include "Player.h"
#include "TextureLoader.h"
Player::Player()
	:idleTexture_R(nullptr), idleTexture_L(nullptr), runTexture_R(nullptr), runTexture_L(nullptr),
	jumpTexture_R(nullptr), jumpTexture_L(nullptr),
	dstRect{ 960,735,32,32 }, velocityY(0.0f), isJumping(false), wasLeft(false),
	groundY(735), currentState(State::Idle), 
	frameIndex(0), lastFrameTime(0),frameInterval(0)
{
}

bool Player::Init(SDL_Renderer* Renderer)
{
	//TextureLoader 클래스 생성하고, 텍스처 로딩
	TextureLoader TextureLoader;
	idleTexture_R = TextureLoader.LoadTexture("assets/idle_right.bmp", Renderer);
	idleTexture_L = TextureLoader.LoadTexture("assets/idle_left.bmp", Renderer);
	runTexture_R = TextureLoader.LoadTexture("assets/run_right.bmp", Renderer);
	runTexture_L = TextureLoader.LoadTexture("assets/run_left.bmp", Renderer);
	jumpTexture_R = TextureLoader.LoadTexture("assets/jump_right.bmp", Renderer);
	jumpTexture_L = TextureLoader.LoadTexture("assets/jump_left.bmp", Renderer);
	
	//텍스처 배열에 추가.
	//CleanUp할 때 편하게 Destroy 할려고.
	Array_Textures = { 
		idleTexture_L,idleTexture_R,
		runTexture_L,runTexture_R,
		jumpTexture_L,jumpTexture_R 
	};

	//텍스처 멀쩡한지 체크.
	//이상이 있는 텍스처 변수는 nullptr로 나오도록 해놨기 때문에,
	for (int i = 0; i < Array_Textures.size(); i++)
	{
		if (Array_Textures[i] == nullptr)
		{
			return false;
		}
	}

	//InitFrames 람다 함수
	//아직 전역에서 사용되지 않을 뿐 더러, Init에서만 간단하게 사용하겠다는 의미.
	auto InitFrames = [](SDL_Rect frames[], int count, int w, int h, int cols, int rows)
		{
			for (int i = 0; i < count; i++)
			{
				int col = i % cols;
				int row = i / rows;
				frames[i] = { col * w, row * h, w,h };
			}
		};

	//각 텍스처에 맞게 프레임 저장.
	InitFrames(idleFrames, 9, 32, 32, 3, 3);
	InitFrames(runFrames, 8, 32, 32, 3, 3);
	InitFrames(jumpFrames, 12, 32, 32, 3, 4);

	return true;
}

void Player::CleanUp()
{
	//로딩했던 텍스처 삭제.
	for (SDL_Texture* Texture : Array_Textures)
	{
		SDL_DestroyTexture(Texture);
	}
}

//인풋 제어
void Player::HandleInput(const Uint8* keyStates)
{
	bool moving = false;

	//좌 우
	if (keyStates[SDL_SCANCODE_A])
	{
		dstRect.x -= 3;
		wasLeft = true;
		moving = true;
	}
	else if (keyStates[SDL_SCANCODE_D])
	{
		dstRect.x += 3;
		wasLeft = false;
		moving = true;
	}

	//점프
	if (keyStates[SDL_SCANCODE_SPACE] && !isJumping)
	{
		isJumping = true;
		velocityY = -8.5f;
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
	WrapAroundScreen(1920);
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
		//점프 애니메이션 구간별로 프레임 정하기
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
	//점프 속도와 중력 적용
	if (isJumping)
	{
		dstRect.y += static_cast<int>(velocityY);
		velocityY += 0.5f; //gravity

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
	//캐릭터 화면 바깥으로 나가면 반대쪽에서 나오도록 하는 로직
	if (dstRect.x + dstRect.w < 0)
	{
		dstRect.x = screenWidth;
	}
	else if (dstRect.x > screenWidth)
	{
		dstRect.x = -dstRect.w;
	}
}
