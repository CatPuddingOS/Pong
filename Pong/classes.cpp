#pragma once
#include <iostream>
#include "classes.h"

inline void Paddle::incrimentAcceleration(char direction)
{
	if (direction == '+')
		acceleration -= 5000.f;
	else if (direction == '-')
		acceleration += 5000.f;
};
inline float Paddle::getXposition()
{
	return Xposition;
};
inline float Paddle::getYposition()
{
	return Yposition;
};
inline void Paddle::movePaddle(float delta, float accel)
{
	/*Paddle motion(slide physics) could be calculated relative to MOUSE displacement
	  rather than a predefined acceleration
		-Acceleration would still play a part but the actual distance the mouse travels in
		 (delta) seconds would play into acceleration*/
	
	/*				useful computations
		Vel = A / D
		Accel = V / D
		Displace = V * D + ((.5) * A * pow(D, 2))
	*/

	acceleration -= Yvelocity * 10.f;
	Yposition += (Yvelocity * delta) + (acceleration * delta * delta * .5f);
	Yvelocity += acceleration * delta;
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
inline void Paddle::printMovementValues()
{
	std::cout << "A: " << acceleration << " # # # V: " << Yvelocity << " # # # P: " << Yposition << " | ";
}