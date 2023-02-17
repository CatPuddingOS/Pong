#pragma once
#include <SDL.h>
class Paddle
{
public:
	float verticalHalfSize; //HalfSize also relates to TOTAL height in our case
	float Xposition, Yposition;
	float previousYPosition, modifiedYPosition;
	float Xvelocity, Yvelocity;
	float acceleration;
	SDL_Rect paddle;

	void CalculateAcceleration(float, float, float);
	float getXposition();
	float getYposition();
	void MovePaddle(float, float);
	void HandleContactingWall(int);

	void printMovementValues();
};

class Ball
{
public:
	float Xposition, Yposition;
	float Xvelocity, Yvelocity;
	bool isTouching;
	SDL_Rect ball;

	void HandleBouncePlayer();
	void HandleBounceWall();
};

class Court
{
public:
	float topBoundary, bottomBoundary, leftBoundary, rightBoundary;
};