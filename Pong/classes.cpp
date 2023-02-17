#pragma once
#include <iostream>
#include "classes.h"

inline void Paddle::CalculateAcceleration(float current, float prev, float delta)
{
	if (prev < current)
		acceleration -= 100000.f * (current - prev) * delta;
	else if (prev > current)
		acceleration += 100000.f * (prev - current) * delta;
		
	std::cout << "acc: " << acceleration << std::endl;
};
inline float Paddle::getXposition()
{
	return Xposition;
};
inline float Paddle::getYposition()
{
	return Yposition;
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