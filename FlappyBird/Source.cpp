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
	// Vertical movement physics
	float fBirdPosition = 0.0f;
	float fBirdVelocity = 0.0f;
	float fBirdAcceleration = 0.0f;
	float fGravity = 100.0f;

	float fSectionWidth;
	list<int> listSection;
	float fLevelPosition = 0.0f;

	bool hasCollided = false;
	bool resetGame = false;

	int nAttemptCount = 0;
	int nFlapCount = 0;
	int nMaxFlapCount = 0;

protected:
	virtual bool OnUserCreate()
	{
		// The display will be split into equally sized vertical strips
		// Each strip will carry an obstacle for flappy bird to overcome
		// Change the length of listSection to change number of obstacles
		// Begin with no obstacles, to give Flappy a chance..!
		listSection = { 0, 0, 0 };
		fSectionWidth = (float)ScreenWidth() / (float)(listSection.size() - 1);
		resetGame = true;
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// At the start of each game loop, check if game needs to reset
		if (resetGame)
		{
			hasCollided = false;
			resetGame = false;
			listSection = { 0, 0, 0, 0 };
			fBirdAcceleration = 0.0f;
			fBirdVelocity = 0.0f;
			fBirdPosition = ScreenHeight() / 2.0f;
			nFlapCount = 0;
			nAttemptCount++;
		}

		// Create "game over" pause screen
		if (hasCollided)
		{
			DrawString(35, 24, "Game Over")
			DrawString(28, 28, "Press space to restart")
			if (m_keys[VK_SPACE].bReleased)
				resetGame = true;
		}
		else
		{
			// Flapping with space key
			// Limit flapping by introducing a condition on current velocity
			if (m_keys[VK_SPACE].bPressed && fBirdVelocity >= fGravity / 10.0f)
			{
				// Enable bird to have upward thrust from flap
				fBirdAcceleration = 0.0f;
				fBirdVelocity = -fGravity / 4.0f;
				// Enable high score to update when exceeded
				nFlapCount++;
				if (nFlapCount > nMaxFlapCount)
					nMaxFlapCount = nFlapCount;
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

			// Speed of screen movement
			fLevelPosition += 14.0f * fElapsedTime;

			// Fix offset and create the "converyer belt" using listSection
			if (fLevelPosition > fSectionWidth)
			{
				fLevelPosition -= fSectionWidth;
				listSection.pop_front();

				// Randomly set the obstacle height
				// Occasionally leave out obstacles to give bird a break :)
				int obsHeight = rand() % (ScreenHeight() - 20);
				if (obsHeight <= 10) obsHeight = 0;
				listSection.push_back(obsHeight);
			}

			// Clear the display before drawing
			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

			// Draw obstacles
			int nSection = 0;
			int gap = 15;
			int obsWidth = 20;
			for (auto s : listSection)
			{
				if (s != 0)
				{
					Fill(nSection * fSectionWidth + gap - fLevelPosition, ScreenHeight() - s, nSection * fSectionWidth + obsWidth - fLevelPosition, ScreenHeight(), PIXEL_SOLID, FG_GREEN);
					Fill(nSection * fSectionWidth + gap - fLevelPosition, 0, nSection * fSectionWidth + obsWidth - fLevelPosition, ScreenHeight() - s - obsWidth, PIXEL_SOLID, FG_GREEN);
				}
				nSection++;
			}

			// Bird's x-axis positioning
			int nBirdX = (int)(ScreenWidth() / 3.0f);

			// Collision Detection
			// Checking whether the bird is too high or low on screen
			// Checking whether any of the 4 corners of the bird have hit something non-empty
			hasCollided = fBirdPosition < 2 || fBirdPosition > ScreenHeight() - 2 ||
				m_bufScreen[(int)(fBirdPosition + 0) * ScreenWidth() + nBirdX].Char.UnicodeChar != L' ' ||
				m_bufScreen[(int)(fBirdPosition + 1) * ScreenWidth() + nBirdX].Char.UnicodeChar != L' ' ||
				m_bufScreen[(int)(fBirdPosition + 0) * ScreenWidth() + nBirdX + 6].Char.UnicodeChar != L' ' ||
				m_bufScreen[(int)(fBirdPosition + 1) * ScreenWidth() + nBirdX + 6].Char.UnicodeChar != L' ';

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

			// Draw score
			DrawString(1, 1, L"Attempt: " + to_wstring(nAttemptCount));
			DrawString(1, 2, L"Score: " + to_wstring(nFlapCount));
			DrawString(1, 3, L"High Score: " + to_wstring(nMaxFlapCount));

			return true;
		}

	}
};

int main()
{
	FlappyBird game;
	game.ConstructConsole(80, 48, 16, 16); // width, height, character pixels
	game.Start();

	return 0;
}
