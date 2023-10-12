#include "Coin.h"

Coin::Coin(float x, float y, int colliderHeight, int colliderWidth, const char* spritePath) : Object(x, y, colliderHeight, colliderWidth, spritePath)
{
	setSpriteSize(sprite, colliderWidth, colliderHeight);
}