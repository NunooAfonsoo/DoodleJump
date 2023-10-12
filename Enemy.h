#pragma once

#include "Object.h"
#include "MoveDirection.cpp"
#include "Character.h"

class Enemy : public Object, Shootable
{
public:
	Enemy(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath);

	void update(float deltaTime, Character* doodle, Bullet*& bullet);

private:
	const char* ENEMY_SPRITE_RIGHT_PATH = "data/bunny-right.png";
	const char* ENEMY_SPRITE_LEFT_PATH = "data/bunny-left.png";
	const char* BULLET_SPRITE_PATH = "data/bullet.png";

	const float SHOT_INTERVAL = 2;

	LookDirection currentDirection;
	float timeSinceLastShot;


	virtual void shoot(Vector2 direction, Bullet*& bullet) override;
};
