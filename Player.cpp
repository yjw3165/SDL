#include "Player.h"
#include "TextureLoader.h"
#include <iostream>

//Constructor
Player::Player()
	:idleTexture_R(nullptr), idleTexture_L(nullptr), runTexture_R(nullptr), runTexture_L(nullptr),
	jumpTexture_R(nullptr), jumpTexture_L(nullptr), attack_L(nullptr), attack_R(nullptr),
	dstRect{ 960,735,32,32 }, velocityY(0.0f),isAttacking(false), isJumping(false), wasLeft(false),
	groundY(735), currentState(State::Idle), 
	frameIndex(0), lastFrameTime(0),frameInterval(0)
{
}

bool Player::Init(SDL_Renderer* Renderer)
{
	//TextureLoader Ŭ���� �����ϰ�, �ؽ�ó �ε�
	TextureLoader TextureLoader;
	idleTexture_R = TextureLoader.LoadTexture("assets/idle_right.bmp", Renderer);
	idleTexture_L = TextureLoader.LoadTexture("assets/idle_left.bmp", Renderer);
	runTexture_R = TextureLoader.LoadTexture("assets/run_right.bmp", Renderer);
	runTexture_L = TextureLoader.LoadTexture("assets/run_left.bmp", Renderer);
	jumpTexture_R = TextureLoader.LoadTexture("assets/jump_right.bmp", Renderer);
	jumpTexture_L = TextureLoader.LoadTexture("assets/jump_left.bmp", Renderer);
	attack_R = TextureLoader.LoadPNGTexture("assets/attack_right.png", Renderer);
	attack_L = TextureLoader.LoadPNGTexture("assets/attack_left.png", Renderer);
	
	//�ؽ�ó �迭�� �߰�.
	//CleanUp�� �� ���ϰ� Destroy �ҷ���.
	Array_Textures = { 
		idleTexture_L,idleTexture_R,
		runTexture_L,runTexture_R,
		jumpTexture_L,jumpTexture_R,
		attack_R,attack_L
	};

	//�ؽ�ó �������� üũ.
	//�̻��� �ִ� �ؽ�ó ������ nullptr�� �������� �س��� ������,
	for (int i = 0; i < Array_Textures.size(); i++)
	{
		if (Array_Textures[i] == nullptr)
		{
			return false;
		}
	}

	//InitFrames ���� �Լ�
	//���� �������� ������ ���� �� ����, Init������ �����ϰ� ����ϰڴٴ� �ǹ�.
	auto InitFrames = [](SDL_Rect frames[], int count, int w, int h, int cols, int rows)
		{
			for (int i = 0; i < count; i++)
			{
				int col = i % cols;
				int row = i / rows;
				frames[i] = { col * w, row * h, w,h };
			}
		};

	//�� �ؽ�ó�� �°� ������ ����.
	InitFrames(idleFrames, 9, 32, 32, 3, 3);
	InitFrames(runFrames, 8, 32, 32, 3, 3);
	InitFrames(jumpFrames, 12, 32, 32, 3, 4);
	InitFrames(AttackFrames, 8, 32, 32, 3, 3);

	return true;
}

void Player::CleanUp()
{
	//�ε��ߴ� �ؽ�ó ����.
	for (SDL_Texture* Texture : Array_Textures)
	{
		SDL_DestroyTexture(Texture);
	}
}

//��ǲ ����
void Player::HandleInput(const Uint8* keyStates)
{
	bool moving = false;

	//�� ��
	if (keyStates[SDL_SCANCODE_LEFT])
	{
		dstRect.x -= 3;
		wasLeft = true;
		moving = true;
	}
	else if (keyStates[SDL_SCANCODE_RIGHT])
	{
		dstRect.x += 3;
		wasLeft = false;
		moving = true;
	}

	//����
	if (keyStates[SDL_SCANCODE_Z] && !isAttacking)
	{
		isAttacking = true;
		frameIndex = 0;
		
	}
	//����
	if (keyStates[SDL_SCANCODE_SPACE] && !isJumping)
	{
		isJumping = true;
		isAttacking = false;
		velocityY = -8.5f;
		frameIndex = 0;
	}

	if (isAttacking)
	{
		currentState = State::Attack;
		if (wasLeft)
		{
			AttackHitbox =
			{
				dstRect.x - 20,
				dstRect.y + 8,
				20,
				16
			};
		}
		else
		{
			AttackHitbox =
			{
				dstRect.x + dstRect.w,
				dstRect.y + 8,
				20,
				16
			};
		}
		return;
	}
	else
	{
		AttackHitbox = { 0,0,0,0 };
		showDebugHitbox = false;
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
	case State::Attack:
		currentFrames = AttackFrames;
		texture = wasLeft ? attack_L : attack_R;
		break;
	}

	if (currentFrames && texture)
	{
		SDL_RenderCopy(Renderer, texture, &currentFrames[frameIndex], &dstRect);
	}

	if (showDebugHitbox)
	{
		SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(Renderer, &AttackHitbox);
	}
}

bool Player::isAttackFrame() const
{
	return currentState == State::Attack && frameIndex == 5;
}

SDL_Rect Player::GetAttackHitBox()
{
	return AttackHitbox;
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
	case State::Attack:
		frames = AttackFrames;
		frameCount = 8;
		frameInterval = 50;
		break;
	}

	//����
	if (isAttacking)
	{
		
		if (currentTime - lastFrameTime >= frameInterval) 
		{
			frameIndex++;
			lastFrameTime = currentTime;

			if (frameIndex == 5)
			{
				showDebugHitbox = true;
			}
			if (frameIndex >= frameCount) 
			{
				frameIndex = 0;
				currentState = State::Idle;
				showDebugHitbox = false;
				isAttacking = false;
			}
			
		}
		return;
	}

	//����
	if (isJumping)
	{
		//���� �ִϸ��̼� �������� ������ ���ϱ�
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
	//���� �ӵ��� �߷� ����
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
	//ĳ���� ȭ�� �ٱ����� ������ �ݴ��ʿ��� �������� �ϴ� ����
	if (dstRect.x + dstRect.w < 0)
	{
		dstRect.x = screenWidth;
	}
	else if (dstRect.x > screenWidth)
	{
		dstRect.x = -dstRect.w;
	}
}
