#include "Object.h"

Object::Object(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath)
{
	position.x = x;
	position.y = y;

	this->colliderHeight = colliderHeight;
	this->colliderWidth = colliderWidth;

	sprite = createSprite(spritePath);
}

Object::~Object()
{
	destroySprite(sprite);
}

void Object::update(float deltaTime)
{
	draw();
}

void Object::updateYPosition(float pixels)
{
	position.y += pixels;
}

void Object::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

Vector2 Object::getPosition()
{
	return position;
}

int Object::getColliderHeight()
{
	return colliderHeight;
}

int Object::getColliderWidth()
{
	return colliderWidth;
}

void Object::draw()
{
	drawSprite(sprite, position.x, position.y);
}