#pragma once
#include <iostream>
#include "classes.h"

inline Paddle::Paddle(float screenHeight, int side) 
	:verticalHalfSize{ screenHeight / 12 },
	Xposition{0.f}, Yposition{0.f}, previousYPosition{0.f}, modifiedYPosition{0.f},
	Xvelocity{0.f}, Yvelocity{0.f},
	acceleration{0.f}
{
	//Working but experimental
	buoyancy = 1.5;

	paddle.w = 15;
	paddle.h = screenHeight / 6;
	paddle.x = side;
	paddle.y = Yposition;
}
inline Paddle::~Paddle()
{}
inline float Paddle::getXposition()
{
	return Xposition;
};
inline float Paddle::getYposition()
{
	return Yposition;
};
//Match sign of acceleration to paddles displacement
inline void Paddle::CalculateAcceleration(float delta)
{
	float displacement = previousYPosition - modifiedYPosition;

	if (displacement > -1)
		acceleration += 50000.f * (modifiedYPosition - previousYPosition) * delta;
	else if (displacement < -1)
		acceleration -= 50000.f * (previousYPosition - modifiedYPosition) * delta;
		
	std::cout << "acc: " << acceleration << std::endl;
};
inline void Paddle::ApplyPhysics(float delta)
{	
	/*calculating friction*/acceleration -= Yvelocity * 10.f;
	/*the next y position*/Yposition += (Yvelocity * delta) + (.5f * acceleration * delta * delta);
	/*Final velocity*/Yvelocity += acceleration * delta;
}
inline void Paddle::MovePaddle(float delta)
{
	//Position with no values applied
	Yposition = (float)modifiedYPosition;
	//Physics calculations
	CalculateAcceleration(delta);
	//Augment the next position
	ApplyPhysics(delta);
	//Done with current position edit
	previousYPosition = modifiedYPosition;

	//For bounce
	acceleration /= buoyancy;
	//For slide
	//acceleration = 0;
}
inline void Paddle::HandleContactingWall(int collisionArea)
{
	if (collisionArea >= 895)
	{
		Yposition = collisionArea - verticalHalfSize;
	}
	else if (collisionArea <= 5)
	{
		Yposition = collisionArea + verticalHalfSize;
	}
	Yvelocity = 0; acceleration = 0;
}
/*---DEBUG---*/
inline void Paddle::printMovementValues()
{
	std::cout << "A: " << acceleration << " # # # V: " << Yvelocity << " # # # P: " << Yposition << " | ";
}

/*BALL*/
inline void HandleBouncePlayer()
{

}
inline void HandleBounceWall()
{

}

/*DELTA TIME AND FRAME*/
inline void DeltaFrames::CalculateDelta()
{
	deltaTime = (SDL_GetTicks() - lastFrame) / 1000.f;
}
inline void DeltaFrames::CheckNewFrame(int lastTime)
{
	lastFrame = SDL_GetTicks64();
	if (lastFrame >= (lastTime + 1000))
	{
		lastTime = lastFrame;
	}
}
//Works independently of delta time to check time between frames
inline void DeltaFrames::SetNextTimerFPS()
{
	timerFPS = SDL_GetTicks64() - lastFrame;
}
//Throttles fps via predefined delay
inline void DeltaFrames::FPSThrottle()
{
	frameCount++;	
	if (deltaTime < (1000 / 60))
	{
		SDL_Delay((1000 / 60) - deltaTime);
	}
}