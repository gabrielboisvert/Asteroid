#include "pch.h"
#include "GameEntity.h"


Asteroid::GameEntity::GameEntity()
{
	this->sprite = nullptr;
}

Asteroid::Sprite* Asteroid::GameEntity::getSprite()
{
	return this->sprite;
}

void Asteroid::GameEntity::setSprite(Sprite* sprite)
{
	this->sprite = sprite;
}