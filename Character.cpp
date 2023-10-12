#include "Character.h"

Character::Character(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath) : Object(x, y, colliderHeight, colliderWidth, spritePath)
{
	abilityActive = false;
	timeOfAbilityUse = 0;
	intervalBetweenShots = MAX_INTERVAL_BETWEEN_SHOTS;
	movedPixelsLastFrame = 0;
}

void Character::update(float deltaTime, Vector2 mouseDirection)
{
	updateDirection(mouseDirection);

	update(deltaTime);
}


void Character::update(float deltaTime)
{
	fall(deltaTime);

	intervalBetweenShots += deltaTime;

	if (abilityActive)
	{
		updateAbilityUseTime(deltaTime);
	}

	draw();
}

void Character::fall(float deltaTime)
{
	movedPixelsLastFrame = verticalVelocity * deltaTime;
	updateYPosition(-movedPixelsLastFrame);
	verticalVelocity -= GRAVITY * deltaTime;
}

void Character::updateAbilityUseTime(float deltaTime)
{
	timeOfAbilityUse += deltaTime;

	if (timeOfAbilityUse >= MAX_ABILITY_TIME)
	{
		abilityActive = false;
	}
}

void Character::Move(LookDirection direction, float deltaTime)
{
	if (direction == LookDirection::LEFT)
	{
		position.x -= 1 * HORIZONTAL_SPEED * deltaTime;
	}
	else
	{
		position.x += 1 * HORIZONTAL_SPEED * deltaTime;
	}
}

void Character::shoot(Vector2 direction, Bullet*& bullet)
{

	if (intervalBetweenShots < MAX_INTERVAL_BETWEEN_SHOTS)
	{
		return;
	}

	int xOffset = 5;
	float xPosition;
	float yPosition;

	if (currentLookDirection == LookDirection::RIGHT)
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

	bullet = new Bullet(xPosition, yPosition, BULLET_COLLIDER_HEIGHT, BULLET_COLLIDER_WIDTH, BULLET_SPRITE_PATH, direction);
	intervalBetweenShots = 0;
}

CollisionDirection Character::getCollisionDirection(Object* a, Object* b)
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
			if (aPosition.y < bPosition.y && !isGoingUp())
			{
				return CollisionDirection::DOWN;
			}
			else if (isGoingUp())
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

void Character::updateDirection(Vector2 direction)
{
	if (direction.x < 0 && currentLookDirection != LookDirection::LEFT)
	{
		destroySprite(sprite);
		sprite = createSprite(SPRITE_LEFT_PATH);
		currentLookDirection = LookDirection::LEFT;
	}
	else if (direction.x > 0 && currentLookDirection != LookDirection::RIGHT)
	{
		destroySprite(sprite);
		sprite = createSprite(SPRITE_RIGHT_PATH);
		currentLookDirection = LookDirection::RIGHT;
	}
}

void Character::jump()
{
	verticalVelocity = JUMP_STRENGTH;
}

void Character::holdYPosition(float yPosition)
{
	position.y = yPosition;
}

void Character::activateAbility()
{
	timeOfAbilityUse = 0;
	abilityActive = true;
}

bool Character::isGoingUp()
{
	return verticalVelocity > 0;
}

bool Character::isAbilityActive()
{
	return abilityActive;
}

float Character::getMovedPixelsLastFrame()
{
	return movedPixelsLastFrame;
}
