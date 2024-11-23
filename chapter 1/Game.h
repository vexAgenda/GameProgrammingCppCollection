#pragma once

#include <SDL.h>
#include <vector>
#include <random>

class SDL_Window;
class SDL_Renderer;

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 mBallPos;
	Vector2 mBallVel;
};

const int thickness{ 15 };

class Game
{
public:
	Game() : mPaddlePos{ {thickness,768 / 2},{1024 - thickness * 2, 768/2} },
		mTicksCount{}
	{}
	//게임 초기화
	bool Initialize();
	// 게임이 끝나기 던까지 게임 루프를 실행
	void RunLoop();
	//게임 종료
	void Shutdown();

private:
	// 게임 루프를 위한 헬퍼 함수
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// SDL이 생성한 윈도우
	SDL_Window* mWindow;

	SDL_Renderer* mRenderer;
	// 게임이 계속 실행되야하는지 판단
	bool mIsRunning{true};

	Vector2 mPaddlePos[2];
	std::vector<Ball> balls;

	Uint32 mTicksCount;
	int mPaddleDir[2];
	int paddleH{ 100 };

	std::random_device rd{};
	std::uniform_real_distribution<float> ball_x{ 1024 / 2.f - 200.f, 1024 / 2.f + 200.f };
	std::uniform_real_distribution<float> ball_y{ 768 / 2.f - 100.f, 768 / 2.f + 100.f };
	std::uniform_real_distribution<float> ballv{ -235.0f, 235.0f };

};

