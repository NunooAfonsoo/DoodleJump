#include "Bullet.h"

Bullet::Bullet(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath, Vector2 direction) : Object(x, y, colliderHeight, colliderWidth, spritePath)
{
	this->direction = direction;
	timeSinceSpawn = 0;
}

void Bullet::update(float deltaTime)
{
	timeSinceSpawn += deltaTime;
	move(deltaTime);

	draw();
}

CollisionDirection Bullet::getCollisionDirection(Object* a, Object* b)
{
	if (timeSinceSpawn < 0.1f)
	{
		return CollisionDirection::NONE;
	}

	return Collidable::getCollisionDirection(a, b);
}

void Bullet::move(float deltaTime)
{
	position += direction * BULLET_SPEED * deltaTime;
}