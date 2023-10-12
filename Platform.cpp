#include "Platform.h"
Platform::Platform(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath) : Object(x, y, colliderHeight, colliderWidth, spritePath)
{
	hasBeenPassedByPlayer = false;
}

void Platform::updateYPosition(float pixels)
{
	Object::updateYPosition(pixels);
}

void Platform::setHasBeenPassedByPlayer()
{
	hasBeenPassedByPlayer = true;
}

bool Platform::getHasBeenPassedByPlayer()
{
	return hasBeenPassedByPlayer;
}