#include "Framework.h"
#include "Character.h"
#include "Platform.h"
#include "MoveablePlatform.h"
#include "Bullet.h"
#include "EnemyPlatform.h"
#include "Coin.h"

#include <vector>
#include <random>
#include <string>

class Game : public Framework {
public:
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = windowWidth;
		height = windowHeight;
		fullscreen = false;

		startingPlatformYPosition = 0.9f * windowHeight;
		highestSessionScore = 0;
	}

	virtual bool Init() 
	{
		gameHasStarted = false;

		vectorsCleanUp();

		initializeNumberSpritesArray();

		background = createSprite(BACKGROUND_SPRITE_PATH);
		setSpriteSize(background, windowWidth, windowHeight);
		activeAbility = createSprite(BULLET_SPRITE_PATH);
		setSpriteSize(activeAbility, ACTIVE_ABILITY_WIDTH, ACTIVE_ABILITY_HEIGHT);

		if (highestSessionScore < currentScore)
		{
			highestSessionScore = currentScore;
		}

		Initialize();

		doodle = new Character(STARTING_PLATFORM_X_POSITION, startingPlatformYPosition - CHARACTER_COLLIDER_HEIGHT - 25, CHARACTER_COLLIDER_HEIGHT, CHARACTER_COLLIDER_WIDTH, DOODLE_SPRITE_PATH);
		spawnPlatform();

		return true;
	}

	virtual void Close()
	{
		cleanUp();
	}

	virtual bool Tick()
	{
		deltaTime = ((float)getTickCount() - previousTicks) / 1000;;
		previousTicks = (float)getTickCount();

		drawSprite(background, 0, 0);

		updatePlatforms();

		doodle->update(deltaTime, mouseDirection);

		showScore(currentScore, scoreNumbers, SCORE_LEFT_X_POSITION, SCORE_Y_POSITION);
		showScore(highestSessionScore, highestScoreNumbers, SCORE_LEFT_X_POSITION, HIGHEST_SCORE_Y_POSITION);
		showScore(numberOfPlatformsPassed, platformsPassed, SCORE_LEFT_X_POSITION, PLATFORMS_SCORE_Y_POSITION);
		showScore(numberOfCoinsGathered, coinsGathered, SCORE_LEFT_X_POSITION, NUMBER_OF_COINS_Y_POSITION);
		checkCollisions();

		if (!gameHasStarted)
		{
			if (doodle->getPosition().y > windowHeight)
			{
				doodle->setPosition(STARTING_PLATFORM_X_POSITION, (float)windowHeight / 2 + 25);
			}
			return false;
		}

		if (moveLeftPressed)
		{
			doodle->Move(LookDirection::LEFT, deltaTime);
		}
		if (moveRightPressed)
		{
			doodle->Move(LookDirection::RIGHT, deltaTime);
		}

		deleteObjectsOutOfBounds();
		checkBulletsPositions();
		checkDoodlePosition();

		updateBullets();
		updateCoins();

		if (doodle->isGoingUp() && doodle->getPosition().y < (float)windowHeight / 2)
		{
			moveCameraWithDoodle();
			doodle->holdYPosition((float)windowHeight / 2);
		}

		if (timeSinceLastCoinSpawn > COIN_SPAWN_TIME && spawnedCoins < MAX_NUMBER_OF_COINS)
		{
			spawnCoin();
			timeSinceLastCoinSpawn = 0;
			spawnedCoins++;
		}

		if (doodle->isAbilityActive())
		{
			autoShootAbility();
			drawSprite(activeAbility, SCORE_LEFT_X_POSITION, SCORE_Y_POSITION * 9);
		}

		if (spawnedCoins < MAX_NUMBER_OF_COINS)
		{
			timeSinceLastCoinSpawn += deltaTime;
		}

		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative)
	{
		mouseDirection.x = x - doodle->getPosition().x;
		mouseDirection.y = y - doodle->getPosition().y;
		mouseDirection.normalize();
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased)
	{
		if (isReleased)
		{
			if (gameHasStarted)
			{
				if (button == FRMouseButton::LEFT)
				{
					Bullet* bullet = nullptr;
					doodle->shoot(mouseDirection, bullet);

					if (bullet != nullptr)
					{
						bullets.push_back(bullet);
					}
				}
				else if (button == FRMouseButton::RIGHT)
				{
					if (numberOfCoinsGathered >= ABILITY_COST && !doodle->isAbilityActive())
					{
						doodle->activateAbility();
						numberOfCoinsGathered -= ABILITY_COST;
					}
				}
			}
			else
			{
				if (button == FRMouseButton::RIGHT)
				{
					for (int i = 0; i < MAX_PLATFORM_NUMBER; i++)
					{
						spawnPlatform();
					}

					gameHasStarted = true;
					timeSinceLastCoinSpawn = 0;
				}
			}
		}
	}

	virtual void onKeyPressed(FRKey k) {
		if (k == FRKey::LEFT)
		{
			moveLeftPressed = true;
		}
		else if (k == FRKey::RIGHT)
		{
			moveRightPressed = true;
		}
	}

	virtual void onKeyReleased(FRKey k)
	{
		if (k == FRKey::LEFT)
		{
			moveLeftPressed = false;
		}
		else if (k == FRKey::RIGHT)
		{
			moveRightPressed = false;
		}
	}

	virtual const char* GetTitle() override
	{
		return "DLDoodleJump";
	}

	void SetWindowSize(int width, int height)
	{
		windowWidth = width;
		windowHeight = height;

		platformXDifferenceMax = width / 2 - PLATFORM_COLLIDER_WIDTH;
	}
	

private:
	#pragma region Consts

	// Sprites paths
	const char* BACKGROUND_SPRITE_PATH = "data/bck@2x.png";
	const char* DOODLE_SPRITE_PATH = "data/blue-lik-right.png";
	const char* PLATFORM_SPRITE = "data/Platform.png";
	const char* ENEMY_PLATFORM_SPRITE = "data/PlatformEnemy.png";
	const char* BULLET_SPRITE_PATH = "data/bullet.png";
	const char* ONE_SPRITE_PATH = "data/1.png";
	const char* TWO_SPRITE_PATH = "data/2.png";
	const char* THREE_SPRITE_PATH = "data/3.png";
	const char* FOUR_SPRITE_PATH = "data/4.png";
	const char* FIVE_SPRITE_PATH = "data/5.png";
	const char* SIX_SPRITE_PATH = "data/6.png";
	const char* SEVEN_SPRITE_PATH = "data/7.png";
	const char* EIGHT_SPRITE_PATH = "data/8.png";
	const char* NINE_SPRITE_PATH = "data/9.png";
	const char* ZERO_SPRITE_PATH = "data/0.png";
	const char* COIN_SPRITE_PATH = "data/Coin.png";

	// Platform
	const float STARTING_PLATFORM_X_POSITION = 50;
	const int PLATFORM_X_DIFFERENCE_MIN = 20;
	const int PLATFORM_Y_DIFFERENCE_MIN = 50;
	const int PLATFORM_Y_DIFFERENCE_MAX = 100;
	const int PLATFORM_COLLIDER_HEIGHT = 17;
	const int PLATFORM_COLLIDER_WIDTH = 50;
	const int MAX_PLATFORM_NUMBER = 15;
	const int MIN_SCORE_TO_SPAWN_ENEMIES = 2500;

	// Character
	const int CHARACTER_COLLIDER_HEIGHT = 60;
	const int CHARACTER_COLLIDER_WIDTH = 46;

	//Coins
	const int COIN_COLLIDER_HEIGHT = 40;
	const int COIN_COLLIDER_WIDTH = 40;
	const int MAX_NUMBER_OF_COINS = 5;
	const int COIN_SPAWN_TIME = 2;
	
	// Scores
	const int NUMBER_WIDTH = 13;
	const int SCORE_LEFT_X_POSITION = 10;
	const int SCORE_Y_POSITION = 10;
	const int HIGHEST_SCORE_Y_POSITION = SCORE_Y_POSITION * 3;
	const int PLATFORMS_SCORE_Y_POSITION = SCORE_Y_POSITION * 5;
	const int NUMBER_OF_COINS_Y_POSITION = SCORE_Y_POSITION * 7;

	// Ability
	const int ACTIVE_ABILITY_Y_POSITION = SCORE_Y_POSITION * 7;
	const int ACTIVE_ABILITY_WIDTH = 20;
	const int ACTIVE_ABILITY_HEIGHT = 20;
	const int ABILITY_COST = 20;

	#pragma endregion

	bool gameHasStarted;
	float previousTicks = 0;;
	float deltaTime = 0;
	int windowWidth;
	int windowHeight;
	bool moveRightPressed;
	bool moveLeftPressed;
	Sprite* background;

	// Objects
	std::vector<Object*> platforms;
	std::vector<Object*> bullets;
	std::vector<Object*> coins;
	std::vector<Sprite*> scoreNumbers;
	std::vector<Sprite*> highestScoreNumbers;
	std::vector<Sprite*> platformsPassed;
	std::vector<Sprite*> coinsGathered;

	//Character
	Character* doodle;
	Sprite* activeAbility;
	Vector2 mouseDirection;
	
	// Platform
	float startingPlatformYPosition;
	int platformXDifferenceMax;
	float lastPlatformXPosition;

	// Scores
	int currentScore;
	int highestSessionScore;
	int numberOfPlatformsPassed;
	Sprite* numberSprites[10];

	// Coins
	int spawnedCoins;
	float timeSinceLastCoinSpawn;
	int numberOfCoinsGathered;


	void cleanUp()
	{
		delete doodle;

		if (background != nullptr)
		{
			destroySprite(background);
		}

		vectorsCleanUp();
	}

	void vectorsCleanUp()
	{
		for (Object* platform : platforms)
		{
			delete platform;
		}
		platforms.clear();

		for (Object* bullet : bullets)
		{
			delete bullet;
		}
		bullets.clear();

		for (Object* coin : coins)
		{
			delete coin;
		}
		coins.clear();
	}

	void Initialize() 
	{
		moveRightPressed = false;
		moveLeftPressed = false;
		lastPlatformXPosition = STARTING_PLATFORM_X_POSITION;
		currentScore = 0;
		numberOfPlatformsPassed = -1;
		spawnedCoins = 0;
		timeSinceLastCoinSpawn = 0;
		numberOfCoinsGathered = 0;
	}

	void initializeNumberSpritesArray()
	{
		const char* dataPath = "data/";
		const char* extension = ".png";

		for (int i = 0; i < 10; i++)
		{
			std::string numberPath = dataPath + std::to_string(i) + extension;
			numberSprites[i] = createSprite(numberPath.c_str());
		}
	}

	void spawnPlatform()
	{
		Platform* p;
		if (platforms.size() == 0)
		{
			p = new Platform(STARTING_PLATFORM_X_POSITION, startingPlatformYPosition, PLATFORM_COLLIDER_HEIGHT, PLATFORM_COLLIDER_WIDTH, PLATFORM_SPRITE);
		}
		else
		{
			int xDiff = getRandomNumberInRange(PLATFORM_X_DIFFERENCE_MIN, platformXDifferenceMax);
			int direction = rand() % 2; // 0 left, 1 right
			int xOffset = 50;
			setNextPlatformXLocation(direction, xDiff, xOffset);

			int yDiff = getRandomNumberInRange(PLATFORM_Y_DIFFERENCE_MIN, PLATFORM_Y_DIFFERENCE_MAX);
			Platform* lastPlatform = (Platform*)platforms.back();
			float y = lastPlatform->getPosition().y - yDiff;

			int platformType = getRandomNumberInRange(1, 10); // 1,2 has enemy, 3-5 moveable, 6-10 normal
			if (currentScore <= MIN_SCORE_TO_SPAWN_ENEMIES)
			{
				platformType += 2; //avoid spawning enemies before 2.5k score
			}

			switch (platformType)
			{
				case 1:
				case 2:
					p = new EnemyPlatform(lastPlatformXPosition, y, PLATFORM_COLLIDER_HEIGHT, PLATFORM_COLLIDER_WIDTH, ENEMY_PLATFORM_SPRITE, doodle);
					break;
				case 3:
				case 4:
				case 5:
					p = new MoveablePlatform(lastPlatformXPosition, y, PLATFORM_COLLIDER_HEIGHT, PLATFORM_COLLIDER_WIDTH, PLATFORM_SPRITE, windowWidth - PLATFORM_COLLIDER_WIDTH - xOffset);
					break;
				default:
					p = new Platform(lastPlatformXPosition, y, PLATFORM_COLLIDER_HEIGHT, PLATFORM_COLLIDER_WIDTH, PLATFORM_SPRITE);
					break;
			}
		}

		platforms.push_back(p);
	}

	void setNextPlatformXLocation(int direction, int xDiff, int xOffset)
	{

		if (direction == 0)
		{
			if (lastPlatformXPosition - xDiff <= xOffset)
			{
				lastPlatformXPosition += xDiff;
			}
			else
			{
				lastPlatformXPosition -= xDiff;
			}
		}
		else
		{
			if (lastPlatformXPosition + xDiff > windowWidth - PLATFORM_COLLIDER_WIDTH - xOffset)
			{
				lastPlatformXPosition -= xDiff;
			}
			else
			{
				lastPlatformXPosition += xDiff;
			}
		}
	}

	int getRandomNumberInRange(int min, int max)
	{
		std::random_device randNumber;
		std::mt19937 gen(randNumber());
		std::uniform_int_distribution<> distr(min, max);

		return distr(gen);
	}

	void updatePlatforms()
	{
		for (Object* platform : platforms)
		{
			if (dynamic_cast<EnemyPlatform*>(platform) != nullptr)
			{
				EnemyPlatform* enemyPlatform = (EnemyPlatform*)platform;
				Bullet* bullet = nullptr;
				enemyPlatform->update(deltaTime, bullet);

				if (bullet != nullptr)
				{
					bullets.push_back(bullet);
				}
			}
			else
			{
				platform->update(deltaTime);
			}

			if (!dynamic_cast<Platform*>(platform)->getHasBeenPassedByPlayer() && doodle->getPosition().y + doodle->getColliderHeight() < platform->getPosition().y)
			{
				dynamic_cast<Platform*>(platform)->setHasBeenPassedByPlayer();
				numberOfPlatformsPassed++;
			}
		}
	}

	void showScore(int score, std::vector<Sprite*> scoreNumbersVector, int startingXPosition, int yPosition)
	{
		for (int i = 1; i < scoreNumbersVector.size(); i++)
		{
			destroySprite(scoreNumbersVector[i]);
		}
		scoreNumbersVector.clear();

		int scoreBackup = score;

		if (score == 0)
		{
			scoreNumbersVector.insert(scoreNumbersVector.begin(), numberSprites[0]);
		}

		while (scoreBackup > 0)
		{
			int digit = scoreBackup % 10;
			scoreNumbersVector.insert(scoreNumbersVector.begin(), numberSprites[digit]);
			scoreBackup /= 10;
		}

		for (int i = 0; i < scoreNumbersVector.size(); i++)
		{
			drawSprite(scoreNumbersVector[i], startingXPosition + i * NUMBER_WIDTH, yPosition);
		}
	}

	void checkCollisions()
	{
		checkPlatformsCollisions();
		checkBulletsCollisions();
		checkCoinsCollisions();
	}

	void checkPlatformsCollisions()
	{
		for (Object* platform : platforms)
		{
			if (dynamic_cast<EnemyPlatform*>(platform) != nullptr)
			{
				EnemyPlatform* enemyPlatform = (EnemyPlatform*)platform;
				Enemy* enemy = enemyPlatform->getEnemy();

				if (enemy == nullptr)
				{
					CollisionDirection collisionDirection = doodle->getCollisionDirection(doodle, enemyPlatform);

					if (collisionDirection == CollisionDirection::DOWN)
					{
						doodle->jump();
					}
				}
				else
				{
					CollisionDirection collisionDirection = doodle->getCollisionDirection(doodle, enemy);

					if (collisionDirection != CollisionDirection::NONE)
					{
						if (collisionDirection == CollisionDirection::DOWN)
						{
							enemyPlatform->enemyDestroyed();
							doodle->jump();
						}
						else
						{
							delete doodle;
							Init();
						}

						return;
					}
				}

			}
			else
			{
				CollisionDirection collisionDirection = doodle->getCollisionDirection(doodle, platform);

				if (collisionDirection == CollisionDirection::DOWN)
				{
					doodle->jump();

					return;
				}
			}
		}
	}

	void checkBulletsCollisions()
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			Bullet* bullet = dynamic_cast<Bullet*>(bullets[i]);
			if (bullet->getCollisionDirection(bullet, doodle) != CollisionDirection::NONE)
			{
				delete doodle;
				Init();

				return;
			}

			for (Object* platform : platforms)
			{
				EnemyPlatform* enemyPlatform = dynamic_cast<EnemyPlatform*>(platform);
				if (enemyPlatform != nullptr)
				{
					Enemy* enemy = enemyPlatform->getEnemy();

					if (enemy != nullptr && bullet->getCollisionDirection(bullet, enemy) != CollisionDirection::NONE)
					{
						enemyPlatform->enemyDestroyed();
						deleteVectorElementAtIndex(bullets, i);

						return;
					}
				}
			}
		}
	}

	void checkCoinsCollisions()
	{
		for (int i = 0; i < coins.size(); i++)
		{
			CollisionDirection collisionDirection = doodle->getCollisionDirection(doodle, coins[i]);

			if (collisionDirection != CollisionDirection::NONE)
			{
				deleteVectorElementAtIndex(coins, i);

				spawnedCoins--;
				numberOfCoinsGathered++;

				return;
			}
		}
	}

	void deleteObjectsOutOfBounds()
	{
		for (int i = 0; i < platforms.size(); i++)
		{
			if (isPositionOutsideOfBounds(platforms[i]->getPosition().y))
			{
				deleteVectorElementAtIndex(platforms, i);
				spawnPlatform();
			}
			else
			{
				break;
			}
		}

		for (int i = 0; i < coins.size(); i++)
		{
			if (isPositionOutsideOfBounds(coins[i]->getPosition().y))
			{
				deleteVectorElementAtIndex(coins, i);
				spawnedCoins--;

				break;
			}
		}
	}

	bool isPositionOutsideOfBounds(int yPosition)
	{
		return yPosition < -5 || yPosition > windowHeight;
	}

	void deleteVectorElementAtIndex(std::vector<Object*>& vector, int i)
	{
		Object* object = vector[i];
		vector.erase(vector.begin() + i);

		delete object;
	}

	void checkBulletsPositions()
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			Bullet* bullet = dynamic_cast<Bullet*>(bullets[i]);
			if (isPositionOutsideOfBounds(bullet->getPosition().y))
			{
				deleteVectorElementAtIndex(bullets, i);
			}
			else
			{
				if (bullet->getPosition().x + (float)bullet->getColliderWidth() / 2 <= 0)
				{
					bullet->setPosition(windowWidth, bullet->getPosition().y);
				}
				else if (bullet->getPosition().x >= windowWidth)
				{
					bullet->setPosition(-(float)bullet->getColliderWidth() / 2, bullet->getPosition().y);
				}
			}
		}
	}

	void checkDoodlePosition()
	{
		if (doodle->getPosition().x  + (float)doodle->getColliderWidth() / 2 <= 0)
		{
			doodle->setPosition(windowWidth - (float)doodle->getColliderWidth() / 2, doodle->getPosition().y);
		}
		else if (doodle->getPosition().x + (float)doodle->getColliderWidth() / 2 >= windowWidth)
		{
			doodle->setPosition(-(float)doodle->getColliderWidth() / 2, doodle->getPosition().y);
		}
		else if (isPositionOutsideOfBounds(doodle->getPosition().y + doodle->getColliderHeight()))
		{
			delete doodle;

			Init();
		}
	}

	void updateBullets()
	{
		for (Object* bullet : bullets)
		{
			dynamic_cast<Bullet*>(bullet)->update(deltaTime);
		}
	}

	void updateCoins()
	{
		for (Object* coin : coins)
		{
			coin->update(deltaTime);
		}
	}

	void moveCameraWithDoodle()
	{
		float pixelsMoved = doodle->getMovedPixelsLastFrame();
		for (Object* platform : platforms)
		{
			if (dynamic_cast<EnemyPlatform*>(platform) != nullptr)
			{
				EnemyPlatform* enemyPlatform = (EnemyPlatform*)platform;
				enemyPlatform->updateYPosition(pixelsMoved);
			}
			else
			{
				platform->updateYPosition(pixelsMoved);
			}
		}

		for (Object* bullet : bullets)
		{
			bullet->updateYPosition(pixelsMoved);
		}

		for (Object* coin : coins)
		{
			coin->updateYPosition(pixelsMoved);
		}

		currentScore += 1;
	}

	void spawnCoin()
	{
		int borderOffset = 50;
		int positionX = getRandomNumberInRange(borderOffset, windowWidth - borderOffset);
		int positionY = getRandomNumberInRange(borderOffset, doodle->getPosition().y - borderOffset);

		Coin* coin = new Coin(positionX, positionY, COIN_COLLIDER_HEIGHT, COIN_COLLIDER_WIDTH, COIN_SPRITE_PATH);

		coins.push_back(coin);
	}
	
	void autoShootAbility()
	{
		for (Object* platform : platforms)
		{
			EnemyPlatform* enemyPlatform = dynamic_cast<EnemyPlatform*>(platform);
			if (enemyPlatform != nullptr)
			{
				Enemy* enemy = enemyPlatform->getEnemy();

				if (enemy != nullptr && enemy->getPosition().y + enemy->getColliderHeight() > 0)
				{
					Bullet* bullet = nullptr;
					Vector2 doodleToEnemy = (enemy->getPosition() - doodle->getPosition());
					doodleToEnemy.normalize();

					doodle->updateDirection(doodleToEnemy);
					doodle->shoot(doodleToEnemy, bullet);

					if (bullet != nullptr)
					{
						bullets.push_back(bullet);
					}
				}
			}
		}
	}
};