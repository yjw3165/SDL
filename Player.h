#pragma once
#include <SDL.h>
#include <array>
class Player
{
public:
	
	//생성자
	Player();

	//초기화
	bool Init(SDL_Renderer* Renderer);

	//정리
	void CleanUp();

	//입력 처리
	void HandleInput(const Uint8* keyStates);

	//렌더링
	void Update(Uint32 CurrentTime);
	void Render(SDL_Renderer* Renderer);

	//상태 : 기본 / 달리기 / 점프
	enum class State { Idle, Run, Jump };

private:
	void UpdateAnimation(Uint32 currentTime);
	void UpdatePhysics();
	void WrapAroundScreen(int screenWidth);

	float velocityY = 0.0f;
	bool isJumping = false;
	bool wasLeft = false;
	int groundY = 0;

	SDL_Texture* idleTexture_R = nullptr;
	SDL_Texture* idleTexture_L = nullptr;
	SDL_Texture* runTexture_R = nullptr;
	SDL_Texture* runTexture_L = nullptr;
	SDL_Texture* jumpTexture_R = nullptr;
	SDL_Texture* jumpTexture_L = nullptr;

	int frameIndex = 0;
	Uint32 lastFrameTime = 0;
	Uint32 frameInterval = 0;
	State currentState;

	SDL_Rect dstRect;
	SDL_Rect idleFrames[9];
	SDL_Rect runFrames[8];
	SDL_Rect jumpFrames[12];

	std::array<SDL_Texture*, 6> Array_Textures;
};

