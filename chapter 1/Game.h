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
	//���� �ʱ�ȭ
	bool Initialize();
	// ������ ������ ������ ���� ������ ����
	void RunLoop();
	//���� ����
	void Shutdown();

private:
	// ���� ������ ���� ���� �Լ�
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// SDL�� ������ ������
	SDL_Window* mWindow;

	SDL_Renderer* mRenderer;
	// ������ ��� ����Ǿ��ϴ��� �Ǵ�
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

