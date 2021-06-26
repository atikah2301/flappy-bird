#include <iostream>
#include <string>
using namespace std;

#include "olcConsoleGameEngine.h"

class FlappyBird : public olcConsoleGameEngine
{
public:
	FlappyBird()
	{
		m_sAppName = L"Flappy Bird";
	}

private:
	float fBirdPosition = 0.0f;
	float fBirdVelocity = 0.0f;
	float fBirdAcceleration = 0.0f;
	
	float fGravity = 100.0f;

protected:
	virtual bool OnUserCreate()
	{
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		if (m_keys[VK_SPACE].bPressed)
		{
			// Enable bird to have upward thrust from flap
			fBirdAcceleration = 0.0f;
			fBirdVelocity = -fGravity / 4.0f;
		}
		else
		{
			// Enable bird to fall due to gravity
			// fGravity considered as jerk
			fBirdAcceleration += fGravity * fElapsedTime;
		}

		// Cap accerlation at gravity
		if (fBirdAcceleration >= fGravity)
		{
			fBirdAcceleration = fGravity;
		}

		// Update speed and direction, and position
		fBirdVelocity += fBirdAcceleration * fElapsedTime;
		fBirdPosition += fBirdVelocity * fElapsedTime;

		// Clear the display before drawing
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

		int nBirdX = (int)(ScreenWidth() / 3.0f);

		// Draw flappy bird
		if (fBirdVelocity > 0)
		{
			// Wings raised when velocity > 0, since falling
			DrawString(nBirdX, fBirdPosition + 0.0f, L"\\\\\\");
			DrawString(nBirdX, fBirdPosition + 1.0f, L"<\\\\\\=Q");
		}
		else
		{
			// Wings low when velocity <= 0, since flying
			DrawString(nBirdX, fBirdPosition + 0.0f, L"<///=Q");
			DrawString(nBirdX, fBirdPosition + 1.0f, L"///");
		}

		return true;
	}
};

int main()
{
	FlappyBird game;
	game.ConstructConsole(80, 48, 16, 16); // width, height, character pixels
	game.Start();

	return 0;
}