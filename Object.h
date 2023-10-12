#pragma once

#include "Framework.h"
#include "Vector2.cpp"

class Object
{
public:
	Object(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath);
	~Object();

	virtual void update(float deltaTime);
	virtual void updateYPosition(float pixels);

	void setPosition(float x, float y);
	Vector2 getPosition();
	int getColliderHeight();
	int getColliderWidth();

protected:
	int colliderHeight;
	int colliderWidth;
	Vector2 position;
	Sprite* sprite;


	void draw();
};