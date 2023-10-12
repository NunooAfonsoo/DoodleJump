#pragma once

#include "Object.h"
#include "Collidable.h"

class Bullet : public Object, public Collidable
{
public:
	Bullet(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath, Vector2 direction);

	virtual void update(float deltaTime) override;
	virtual CollisionDirection getCollisionDirection(Object* a, Object* b) override;

	void move(float deltaTime);

private:
	Vector2 direction;
	const float BULLET_SPEED = 1000;
	float timeSinceSpawn;
};