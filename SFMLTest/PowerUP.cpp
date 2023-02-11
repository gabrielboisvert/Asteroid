#include "pch.h"
#include "PowerUP.h"
#include "GameManager.h"

Asteroid::PowerUP::PowerUP(Sprite* sprite)
{
	this->sprite = sprite;
}

Asteroid::PowerUP::~PowerUP()
{
	GameManager::getInstance().getRenderer().removeSprite(this->sprite);
	delete this->sprite;
}
