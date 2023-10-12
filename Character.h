#pragma once

#include "Framework.h"
#include "Object.h"
#include "Moveable.h"
#include "Shootable.h"
#include "Collidable.h"
#include "Vector2.cpp"
#include "MoveDirection.cpp"
#include "Bullet.h"

class Character : public Object, Moveable, Shootable, Collidable
{
public:
	Character(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath);

	void update(float deltaTime, Vector2 mouseDirection);
	virtual void update(float deltaTime) override;
	virtual void Move(LookDirection direction, float deltaTime) override;
	virtual void shoot(Vector2 direction, Bullet*& bullet) override;
	virtual CollisionDirection getCollisionDirection(Object* a, Object* b) override;

	void updateDirection(Vector2 mouseDirection);
	void jump();
	void holdYPosition(float yPosition);
	void activateAbility();
	bool isGoingUp();
	bool isAbilityActive();
	float getMovedPixelsLastFrame();

private:
	const char* SPRITE_RIGHT_PATH = "data/blue-lik-right.png";
	const char* SPRITE_LEFT_PATH = "data/blue-lik-left.png";
	const char* BULLET_SPRITE_PATH = "data/bullet.png";

	const int BULLET_COLLIDER_HEIGHT = 5;
	const int BULLET_COLLIDER_WIDTH = 5;

	const float HORIZONTAL_SPEED = 500;
	const float GRAVITY = 500;
	const float JUMP_STRENGTH = 350;
	const float MAX_ABILITY_TIME = 20;
	const float MAX_INTERVAL_BETWEEN_SHOTS = 0.4;

	LookDirection currentLookDirection = LookDirection::RIGHT;
	float verticalVelocity = 0;
	float movedPixelsLastFrame;
	float intervalBetweenShots;
	bool abilityActive;
	float timeOfAbilityUse;


	void fall(float deltaTime);
	void updateAbilityUseTime(float deltaTime);
};

