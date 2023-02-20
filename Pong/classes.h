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
	/*Experimental: adds wobble to the paddle.The lower the number the more the paddle wobbles. 
	Can be any single decimal from 1.1. Paddle has no bounce at ~4*/
	float buoyancy;
	SDL_Rect paddle;

	Paddle(float, int);
	~Paddle();

	float getXposition();
	float getYposition();
	void CalculateAcceleration(float);
	void ApplyPhysics(float);
	void MovePaddle(float);
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

class DeltaFrames
{
public:
	//intergers are resulting in shitty delta calculations but whatever
	int frameCount, timerFPS, thisFrame, lastFrame, fps;
	float deltaTime;

	void CalculateDelta();
	void CheckNewFrame(int);
	void SetNextTimerFPS();
	void FPSThrottle();
};