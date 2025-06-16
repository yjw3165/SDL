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

	float velocityY;
	bool isJumping;
	bool wasLeft;
	int groundY;

	SDL_Texture* idleTexture_R;
	SDL_Texture* idleTexture_L;
	SDL_Texture* runTexture_R;
	SDL_Texture* runTexture_L;
	SDL_Texture* jumpTexture_R;
	SDL_Texture* jumpTexture_L;

	int frameIndex;
	Uint32 lastFrameTime;
	Uint32 frameInterval;
	State currentState;

	SDL_Rect dstRect;
	SDL_Rect idleFrames[9];
	SDL_Rect runFrames[8];
	SDL_Rect jumpFrames[12];

	std::array<SDL_Texture*, 6> Array_Textures;
};

