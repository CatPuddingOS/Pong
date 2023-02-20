#pragma once
#include <iostream>
#include "classes.h"

inline Paddle::Paddle(float screenHeight, int side)
	:verticalHalfSize{ screenHeight / 12 },
	position(0, 0), lastPosition(0, 0), modifiedPosition(0, 0),
	velocity(0, 0),
	//acceleration should be a vector
	acceleration{0.f}
{
	//Working but experimental
	buoyancy = 1.2;

	paddle.w = 15;
	paddle.h = screenHeight / 6;
	paddle.x = side;
	paddle.y = position.Y;
}
inline Paddle::~Paddle()
{}
inline float Paddle::getXposition()
{
	return position.X;
};
inline float Paddle::getYposition()
{
	return position.Y;
};
//Match sign of acceleration to paddles displacement
inline void Paddle::CalculateAcceleration(float delta)
{
	float displacement = lastPosition.Y - modifiedPosition.Y;

	if (displacement > 0)
		acceleration -= 50000.f * (modifiedPosition.Y - lastPosition.Y) * delta;
	else if (displacement < 0)
		acceleration += 50000.f * (lastPosition.Y - modifiedPosition.Y) * delta;
		
	std::cout << "acc: " << acceleration << std::endl;
};
inline void Paddle::ApplyPhysics(float delta)
{	
	/*calculating friction*/acceleration -= velocity.Y * 10.f;
	/*the next y position*/position.Y += (velocity.Y * delta) + (.5f * acceleration * delta * delta);
	/*Final velocity*/velocity.Y += acceleration * delta;
}
inline void Paddle::MovePaddle(float delta)
{

	//For bounce
	acceleration /= buoyancy;
    //For slide
	//acceleration = 0;
	//Position with no values applied
	position.Y = modifiedPosition.Y;
	//Physics calculations
	CalculateAcceleration(delta);
	//Augment the next position
	ApplyPhysics(delta);
	//Done with current position edit
	lastPosition.Y = modifiedPosition.Y;


}
inline void Paddle::HandleContactingWall(int collisionArea)
{
	if (collisionArea >= 895)
	{
		position.Y = collisionArea - verticalHalfSize;
	}
	else if (collisionArea <= 5)
	{
		position.Y = collisionArea + verticalHalfSize;
	}
	velocity.Y = 0; acceleration = 0;
}
/*---DEBUG---*/
inline void Paddle::printMovementValues()
{
	std::cout << "A: " << acceleration << " # # # V: " << velocity.Y << " # # # P: " << position.Y << " | ";
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