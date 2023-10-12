#pragma once
#include "MoveDirection.cpp"

class Moveable 
{
public:
	virtual void Move(LookDirection direction, float deltaTime) = 0;
};