#pragma once
#include <iostream>
#include "classes.h"

inline float Paddle::getXposition()
{
	return Xposition;
};
inline float Paddle::getYposition()
{
	return Yposition;
};
inline void Paddle::CalculateAcceleration(float current, float prev, float delta)
{
	if (prev < current)
		acceleration -= 100000.f * (current - prev) * delta;
	else if (prev > current)
		acceleration += 100000.f * (prev - current) * delta;
		
	std::cout << "acc: " << acceleration << std::endl;
};
inline void Paddle::MovePaddle(float delta, float accel)
{	
	/*calculating friction*/acceleration -= Yvelocity * 10.f;
	/*the next y position*/Yposition += (Yvelocity * delta) + (.5f * acceleration * delta * delta);
	/*Final velocity*/Yvelocity += acceleration * delta;
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
	Yvelocity = 0;
}
/*---DEBUG---*/
inline void Paddle::printMovementValues()
{
	std::cout << "A: " << acceleration << " # # # V: " << Yvelocity << " # # # P: " << Yposition << " | ";
}

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
	
	if (timerFPS < (1000 / 60))
	{
		SDL_Delay((1000 / 60) - timerFPS);
	}
}