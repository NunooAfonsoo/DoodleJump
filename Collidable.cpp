#include "Collidable.h"

CollisionDirection Collidable::getCollisionDirection(Object* a, Object* b)
{
	Vector2 aPosition = a->getPosition();
	Vector2 bPosition = b->getPosition();

	if (aPosition.x + a->getColliderWidth() < bPosition.x || aPosition.x > bPosition.x + b->getColliderWidth() || aPosition.y + a->getColliderHeight() < bPosition.y || aPosition.y > bPosition.y + b->getColliderHeight())
	{
		return CollisionDirection::NONE;
	}
	else
	{
		if (aPosition.y + a->getColliderHeight() > bPosition.y && aPosition.y + a->getColliderHeight() < bPosition.y + b->getColliderHeight())
		{
			if (aPosition.y < bPosition.y)
			{
				return CollisionDirection::DOWN;
			}
			else
			{
				return CollisionDirection::UP;
			}
		}
		else if (aPosition.x + a->getColliderWidth() > bPosition.x && aPosition.x + a->getColliderWidth() < bPosition.x + b->getColliderWidth())
		{
			if (aPosition.x < bPosition.x)
			{
				return CollisionDirection::RIGHT;
			}
			else
			{
				return CollisionDirection::LEFT;
			}
		}
		else
		{
			return CollisionDirection::NONE;
		}
	}
}