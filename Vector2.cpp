#pragma once
#include <iostream>

struct Vector2
{
	float x;
	float y;

	void normalize() 
	{
		float vectorLength = sqrt(x * x + y * y);
		x /= vectorLength;
		y /= vectorLength;
	}

	Vector2 operator+=(Vector2 a) 
	{
		return { x += a.x, y += a.y };
	}

	Vector2 operator-(Vector2 a)
	{
		return { x - a.x, y - a.y };
	}

	Vector2 operator/(float a)
	{
		return { x/a, y/a };
	}

	Vector2 operator*(float a)
	{
		return { x * a, y * a };
	}

	Vector2 operator/=(float a)
	{
		return { x /= a, y /= a };
	}
};