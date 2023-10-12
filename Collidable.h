#pragma once

#include "CollisionDirection.cpp"
#include "Object.h"

class Collidable
{
public:
	virtual CollisionDirection getCollisionDirection(Object* a, Object* b);
};
