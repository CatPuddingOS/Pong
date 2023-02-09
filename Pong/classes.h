#pragma once
#include <SDL.h>
class Paddle
{
public:
	float verticalHalfSize; //HalfSize also relates to TOTAL height in our case
	float Xposition, Yposition;
	float Xvelocity, Yvelocity;
	float acceleration;

	void incrimentAcceleration(char); //direction takes either + or -
	float getXposition();
	float getYposition();
	float movePaddle(float, float);
	void HandleContactingWall(int);

	void printMovementValues();
};

class LeftPaddle : public Paddle
{
public:
	SDL_Rect leftPaddle;
};

class RightPaddle : public Paddle
{
public:
	SDL_Rect rightPaddle;
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