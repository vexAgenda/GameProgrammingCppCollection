#include "Game.h"

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)",
		100, // �������� ���� ��� x��ǥ
		100, // �������� ���� ��� y��ǥ
		1024, // ������ �ʺ�
		768, // ������ ����
		0 // �÷��� (0�� �ƹ��� �÷��׵� ���� x)
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	for (int i = 0; i < 3; ++i)
	{
		Ball ball;
		balls.push_back(ball);
	}
	for (auto& ball : balls)
	{
		ball.mBallPos = { ball_x(rd),ball_y(rd) };
		ball.mBallVel = {ballv(rd),ballv(rd)};

	}

	return true;
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		//���⼭ ���� ������ �̺�Ʈ�� �ٷ��.
		//�̺�Ʈ�� SDL_QUIT�̸� ������ �����Ѵ�.
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	mPaddleDir[0] = 0;
	mPaddleDir[1] = 0;
	//Ű������ ���� ���
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// �̽������� Ű�� �������� ����
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir[0] -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir[0] += 1;
	}
	if (state[SDL_SCANCODE_I])
	{
		mPaddleDir[1] -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddleDir[1] += 1;
	}

}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
	// ��Ÿ �ð��� ���� ������ ƽ ���� ���� ������ ƽ ���� ���̴�.
	// (���� �� ������ ��ȯ)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// ���� �������� ���� ƽ �� ����
	mTicksCount = SDL_GetTicks();
	//�ִ� ��Ÿ �ð� ������ ����
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	if (mPaddleDir[0] != 0 || mPaddleDir[1] != 0)
	{
		mPaddlePos[0].y += mPaddleDir[0] * 300.0f * deltaTime;
		mPaddlePos[1].y += mPaddleDir[1] * 300.0f * deltaTime;
		if (mPaddlePos[0].y < paddleH / 2.0f + thickness)
		{
			mPaddlePos[0].y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos[0].y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos[0].y = 768.0f - paddleH / 2.0f - thickness;
		}
		if (mPaddlePos[1].y < paddleH / 2.0f + thickness)
		{
			mPaddlePos[1].y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos[1].y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos[1].y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	for (auto& ball : balls)
	{
		ball.mBallPos.x += ball.mBallVel.x * deltaTime;
		ball.mBallPos.y += ball.mBallVel.y * deltaTime;

		if (ball.mBallPos.y <= thickness && ball.mBallVel.y < 0.0f
			|| ball.mBallPos.y >= 768 - thickness && ball.mBallVel.y > 0.0f)
		{
			ball.mBallVel.y *= -1;
		}
		if (ball.mBallPos.x <= 0 && ball.mBallVel.x < 0.0f
			|| ball.mBallPos.x >= 1024 - thickness && ball.mBallVel.x > 0.0f)
		{
			ball.mBallVel.x *= -1;
		}

		int diff = SDL_abs(ball.mBallPos.y - mPaddlePos[0].y);

		if (
			diff <= paddleH / 2.0f &&
			ball.mBallPos.x <= 25.0f && ball.mBallPos.x >= 20.0f &&
			ball.mBallVel.x < 0.0f
			)
		{
			ball.mBallVel.x *= -1.0f;
		}

		diff = SDL_abs(ball.mBallPos.y - mPaddlePos[1].y);

		if (
			diff <= paddleH / 2.0f &&
			ball.mBallPos.x <= 1004.0f && ball.mBallPos.x >= 999.0f &&
			ball.mBallVel.x > 0.0f
			)
		{
			ball.mBallVel.x *= -1.0f;
		}
	}
}
void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255
	);

	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	SDL_Rect wall{
	0, // ���� ��� x
	0, // ���� ��� y
	1024, // �ʺ�
	thickness // ����
	};
	SDL_Rect wall2{
		0, // ���� ��� x
		768 - thickness, // ���� ��� y
		1024, // �ʺ�
		thickness // ����
	};

	SDL_RenderFillRect(mRenderer, &wall);
	SDL_RenderFillRect(mRenderer, &wall2);

	SDL_Rect paddle{
	mPaddlePos[0].x,
	static_cast<int>(mPaddlePos[0].y - paddleH / 2),
	thickness,
	paddleH
	};

	SDL_Rect paddle2{
	mPaddlePos[1].x,
	static_cast<int>(mPaddlePos[1].y - paddleH / 2),
	thickness,
	paddleH
	};
	for (const auto& ball : balls)
	{
		SDL_Rect Ball{
		static_cast<int>(ball.mBallPos.x - thickness / 2),
		static_cast<int>(ball.mBallPos.y - thickness / 2),
		thickness,
		thickness
		};
		SDL_RenderFillRect(mRenderer, &Ball);
	}
	SDL_RenderFillRect(mRenderer, &paddle);
	SDL_RenderFillRect(mRenderer, &paddle2);

	SDL_RenderPresent(mRenderer);

}
