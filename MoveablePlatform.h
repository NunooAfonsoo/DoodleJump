#pragma once

#include "Platform.h"
#include "MoveDirection.cpp"
#include "Moveable.h"

class MoveablePlatform : public Platform, Moveable
{
public:
	MoveablePlatform(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath, int maxXPosition);
	virtual void update(float deltaTime) override;

private:
	const float MOVE_SPEED = 100;

	int maxXPosition;
	int moveDirection; // 0 left, 1 right


	virtual void Move(LookDirection direction, float deltaTime) override;
};

