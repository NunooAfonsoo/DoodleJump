#include "Platform.h"
#include "Character.h"
#include "Enemy.h"

class EnemyPlatform : public Platform
{
public:
	EnemyPlatform(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath, Character* doodle);
	~EnemyPlatform();

	virtual void updateYPosition(float pixels) override;

	void update(float deltaTime, Bullet*& bullet);
	void enemyDestroyed();
	Enemy* getEnemy();

private:
	const char* PLATFORM_SPRITE = "data/Platform.png";
	const char* ENEMY_SPRITE_RIGHT_PATH = "data/bunny-right.png";

	const int ENEMY_COLLIDER_HEIGHT = 58;
	const int ENEMY_COLLIDER_WIDTH = 46;

	Character* doodle;
	Enemy* enemy;
};

