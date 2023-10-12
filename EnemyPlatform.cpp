#include "EnemyPlatform.h"

EnemyPlatform::EnemyPlatform(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath, Character* doodle) : Platform(x, y, colliderHeight, colliderWidth, spritePath)
{
	enemy = new Enemy(x, y - ENEMY_COLLIDER_HEIGHT, ENEMY_COLLIDER_HEIGHT, ENEMY_COLLIDER_WIDTH, ENEMY_SPRITE_RIGHT_PATH);
	this->doodle = doodle;
}

EnemyPlatform::~EnemyPlatform()
{
	delete enemy;
	enemy = nullptr;
}

void EnemyPlatform::updateYPosition(float pixels)
{
	if (enemy != nullptr)
	{
		enemy->updateYPosition(pixels);
	}

	Platform::updateYPosition(pixels);
}

void EnemyPlatform::update(float deltaTime, Bullet*& bullet)
{
	if (enemy != nullptr)
	{
		enemy->update(deltaTime, doodle, bullet);
	}

	Object::update(deltaTime);
}

void EnemyPlatform::enemyDestroyed()
{
	delete enemy;
	enemy = nullptr;

	destroySprite(sprite);
	sprite = createSprite(PLATFORM_SPRITE);
}

Enemy* EnemyPlatform::getEnemy()
{
	return enemy;
}