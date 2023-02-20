#pragma once
#include "class_vector.h"

class Transformation
{
public:
	float X, Y;

	Transformation(float x, float y) : X(x), Y(y) {};
private:
	inline void TranslateX() {};
	inline void TranslateY() {};
	inline void Translate(DimensionalVector vector)
	{
		X += vector.X;
		Y += vector.Y;
	};

};