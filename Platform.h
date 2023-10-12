#pragma once

#include "Framework.h"
#include "Object.h"

class Platform : public Object
{
public:
	Platform(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath);

	virtual void updateYPosition(float pixels) override;

	void setHasBeenPassedByPlayer();
	bool getHasBeenPassedByPlayer();

private :
	bool hasBeenPassedByPlayer;
};
