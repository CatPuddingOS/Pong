#pragma once
class DimensionalVector
{
public:
	float X, Y;

	DimensionalVector(float x = 0, float y = 0) : X(x), Y(y) {};

	//Overload for vector addition
	inline DimensionalVector operator+(const DimensionalVector& v2) const
	{
		return DimensionalVector(X + v2.X, Y + v2.Y);
	};
	//Overload for vector subtraction
	inline DimensionalVector operator-(const DimensionalVector& v2) const
	{
		return DimensionalVector(X - v2.X, Y - v2.Y);
	};
	//Overload for vector multiplication
	inline DimensionalVector operator*(const DimensionalVector& v2) const
	{
		return DimensionalVector(X * v2.X, Y * v2.Y);
	};
	//Overload for vector division
	inline DimensionalVector operator/(const DimensionalVector& v2) const
	{
		return DimensionalVector(X / v2.X, Y / v2.Y);
	};

private:

};