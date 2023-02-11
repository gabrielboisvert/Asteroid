#include "pch.h"
#include "Bullet.h"
#include "GameManager.h"

Asteroid::Bullet::Bullet(Sprite* sprite)
{
	this->sprite = sprite;
}

void Asteroid::Bullet::move(float delta, float speed)
{
	this->sprite->translate(delta * speed, 0);
}

Asteroid::Bullet::~Bullet()
{
	Renderer& renderer = GameManager::getInstance().getRenderer();
	renderer.removeSprite(this->sprite);
}
