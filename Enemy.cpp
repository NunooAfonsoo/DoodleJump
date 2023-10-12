#include "Enemy.h"

Enemy::Enemy(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath) : Object(x, y, colliderHeight, colliderWidth, spritePath)
{
	currentDirection = LookDirection::LEFT;
	timeSinceLastShot = SHOT_INTERVAL;
}

void Enemy::update(float deltaTime, Character* doodle, Bullet*& bullet)
{
	if (doodle->getPosition().x < position.x)
	{
		destroySprite(sprite);
		sprite = createSprite(ENEMY_SPRITE_LEFT_PATH);
		currentDirection = LookDirection::LEFT;
	}
	else
	{
		destroySprite(sprite);
		sprite = createSprite(ENEMY_SPRITE_RIGHT_PATH);
		currentDirection = LookDirection::RIGHT;
	}
	
	if (position.y + colliderHeight > 0)
	{
		timeSinceLastShot -= deltaTime;
		if (timeSinceLastShot <= 0)
		{
			Vector2 enemyToDoodle = (doodle->getPosition() - position);
			enemyToDoodle.normalize();
			shoot(enemyToDoodle, bullet);

			timeSinceLastShot = SHOT_INTERVAL;
		}
	}
	
	Object::update(deltaTime);
}

void Enemy::shoot(Vector2 direction, Bullet*& bullet)
{
	int xOffset = 7;
	float xPosition;
	float yPosition;

	if (currentDirection == LookDirection::RIGHT)
	{
		if (direction.x < 0)
		{
			return;
		}

		xOffset = 5;
		xPosition = position.x + getColliderWidth() + xOffset;
	}
	else
	{
		if (direction.x > 0)
		{
			return;
		}

		xPosition = position.x - xOffset;
	}
	yPosition = position.y + getColliderHeight() / 2;

	bullet = new Bullet(xPosition, yPosition, 5, 5, BULLET_SPRITE_PATH, direction);
}