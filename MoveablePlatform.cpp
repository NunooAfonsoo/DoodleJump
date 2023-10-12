#include "MoveablePlatform.h"

MoveablePlatform::MoveablePlatform(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath, int maxXPosition) : Platform(x, y, colliderHeight, colliderWidth, spritePath)
{
	this->maxXPosition = maxXPosition;
	moveDirection = 0;
}

void MoveablePlatform::update(float deltaTime)
{
	Move(LookDirection(), deltaTime);

	Platform::update(deltaTime);
}

void MoveablePlatform::Move(LookDirection direction, float deltaTime)
{
	if (moveDirection == 0)
	{
		if (position.x > 5)
		{
			position.x -= MOVE_SPEED * deltaTime;
		}
		else
		{
			moveDirection = 1;
		}
	}
	else
	{
		if (position.x < maxXPosition)
		{
			position.x += MOVE_SPEED * deltaTime;
		}
		else
		{
			moveDirection = 0;
		}
	}

}
