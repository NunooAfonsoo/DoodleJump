#pragma once

#include"Object.h"

class Coin : public Object
{
public:
	Coin(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath);
};

