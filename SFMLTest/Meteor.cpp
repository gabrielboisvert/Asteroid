#include "pch.h"
#include "Meteor.h"
#include "GameManager.h"

#include <cmath>

Asteroid::Meteor::Meteor(sf::Texture* texture, MeteorConfig config, sf::Vector2i position)
{
	this->config = config;
	Renderer& renderer = GameManager::getInstance().getRenderer();

	this->sprite = renderer.createSprite(*texture, position, 4);
	this->sprite->scale((config.getRadius() * 2) / this->sprite->getWidth(), (config.getRadius() * 2) / this->sprite->getHeight());
	
	this->cercle = sf::CircleShape(float(config.getRadius()));
	this->cercle.setPosition(this->sprite->getPosition());

	this->selfRotation = 0;
	this->angleDirection = 0;
	this->speed = 0;
}

void Asteroid::Meteor::initMouvement(float selfRotation, float angleDirection, float speed)
{
	this->selfRotation = selfRotation;
	this->angleDirection = angleDirection;
	this->speed = speed;
}

void Asteroid::Meteor::rotate(float delta)
{
	this->sprite->rotate(this->selfRotation * delta);
}

void Asteroid::Meteor::translate(float x, float y)
{
	this->sprite->translate(x, y);
	this->cercle.move(x, y);
}

void Asteroid::Meteor::move()
{
	float delta = GameManager::getInstance().getDeltaTime();

	float rad = this->angleDirection * (3.1415f / 180.0f);

	this->rotate(delta);
	this->translate((cos(rad) * this->speed) * delta, (sin(rad) * this->speed) * delta);
}

sf::CircleShape& Asteroid::Meteor::getCercle()
{
	return this->cercle;
}

Asteroid::MeteorConfig& Asteroid::Meteor::getConfig()
{
	return this->config;
}

Asteroid::Meteor::~Meteor()
{
	GameManager::getInstance().getRenderer().removeSprite(this->sprite);
	delete this->sprite;
}