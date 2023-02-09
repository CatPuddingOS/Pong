#pragma once
#include <iostream>
#include "classes.h"

inline void Paddle::incrimentAcceleration(char direction)
{
	if (direction == '+')
		acceleration -= 15000.f;
	else if (direction == '-')
		acceleration += 15000.f;
};
inline float Paddle::getXposition()
{
	return Xposition;
};
inline float Paddle::getYposition()
{
	return Yposition;
};
inline float Paddle::movePaddle(float delta, float accel)
{
	acceleration -= Yvelocity * 10.f;
	Yposition += (Yvelocity * delta) + (acceleration * delta * delta * .5f);
	Yvelocity += acceleration * delta;

	return Yposition;
}
inline void Paddle::HandleContactingWall(int collisionArea)
{
	if (collisionArea >= 895)
	{
		Yposition = collisionArea - verticalHalfSize;
	}
	else
	{
		Yposition = collisionArea + verticalHalfSize;
	}
	Yvelocity = 0;
}
inline void Paddle::printMovementValues()
{
	std::cout << "A: " << acceleration << " # # # V: " << Yvelocity << " # # # P: " << Yposition << " | ";
}