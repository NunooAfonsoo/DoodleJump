#pragma once
#include "Bullet.h"

class Shootable
{
public:
	virtual void shoot(Vector2 direction, Bullet*& bullet) = 0;
};