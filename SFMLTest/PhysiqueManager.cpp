#include "pch.h"
#include "PhysiqueManager.h"
#include "GameManager.h"
#include <iostream>

void Asteroid::PhysiqueManager::init()
{
	this->player.init();
	this->meteorManager.init();
	this->powerUpManager.init();

	sf::Vector2f screenSize = GameManager::getInstance().getRenderer().getWindow()->getWindow().getView().getSize();
	sf::FloatRect screenArea(0, 0, screenSize.x, screenSize.y);
	this->area = QuadTree(0, screenArea);
}

void Asteroid::PhysiqueManager::update()
{
	GameState state = GameManager::getInstance().getState();
	if (state == GameState::MAIN_MENU || state == GameState::HIGH_SCORE)
		this->player.idleMainMenu();
	else if (state == GameState::RUNNING)
	{
		this->player.move();
		this->meteorManager.move();
		this->powerUpManager.move();

		this->checkCollision();
	}
}

void Asteroid::PhysiqueManager::checkCollision()
{
	this->area.clear();
	std::vector<Meteor*> meteors = this->meteorManager.getMeteors();
	for (unsigned int i = 0; i < meteors.size(); i++)
		this->area.insert(meteors[i]);

	std::vector<Meteor*> meteorClose;
	this->area.retrieve(meteorClose, this->player.getSprite()->getGlobalBound());
	for (unsigned int i = 0; i < meteorClose.size(); i++)
	{
		if (this->collide(this->player.getSprite(), meteorClose[i]))
		{
			this->area.removeMeteor(meteorClose[i]);
			this->meteorManager.destroy(meteorClose[i]);
			this->player.hit();

			GameState state = GameManager::getInstance().getState();
			if (state == GameState::GAME_OVER || state == GameState::HIGH_SCORE)
				return;
		}
	}

	for (unsigned int i = 0; i < this->player.getBullets().size(); i++)
	{
		meteorClose.clear();
		Bullet* bullet = this->player.getBullets()[i];
		this->area.retrieve(meteorClose, bullet->getSprite()->getGlobalBound());
		
		for (unsigned int j = 0; j < meteorClose.size(); j++)
			if (this->collide(bullet->getSprite(), meteorClose[j]))
			{
				this->player.getBullets().erase(std::find(this->player.getBullets().begin(), this->player.getBullets().end(), bullet));
				this->meteorManager.hit(meteorClose[j]);
				delete bullet;
				break;
			}
	}

	for (unsigned int i = 0; i < this->powerUpManager.getPowerUP().size(); i++)
	{
		PowerUP* powerUP = this->powerUpManager.getPowerUP()[i];
		if (this->player.getSprite()->getGlobalBound().intersects(powerUP->getSprite()->getGlobalBound()))
		{
			this->powerUpManager.removePowerUP(powerUP);
			break;
		}
	}
}

bool Asteroid::PhysiqueManager::collide(Sprite* sp, Meteor* meteor)
{
	float circleDistanceX = abs(meteor->getSprite()->getCenter().x - sp->getCenter().x);
	float circleDistanceY = abs(meteor->getSprite()->getCenter().y - sp->getCenter().y);

	if (circleDistanceX > (sp->getWidth() / 2 + meteor->getCercle().getRadius())) { return false; }
	if (circleDistanceY > (sp->getHeight() / 2 + meteor->getCercle().getRadius())) { return false; }

	if (circleDistanceX <= (sp->getWidth() / 2)) { return true; }
	if (circleDistanceY <= (sp->getHeight() / 2)) { return true; }

	float dist = ((circleDistanceX - sp->getWidth() / 2) * (circleDistanceX - sp->getWidth() / 2)) 
					+ ((circleDistanceY - sp->getHeight() / 2) * (circleDistanceY - sp->getHeight() / 2));

	return (dist <= (meteor->getCercle().getRadius() * meteor->getCercle().getRadius()));
}

Asteroid::Player& Asteroid::PhysiqueManager::getPlayer()
{
	return this->player;
}

Asteroid::MeteorManager& Asteroid::PhysiqueManager::getMeteorManager()
{
	return this->meteorManager;
}

Asteroid::PowerUPManager& Asteroid::PhysiqueManager::getPowerUPManager()
{
	return this->powerUpManager;
}

Asteroid::QuadTree& Asteroid::PhysiqueManager::getQuad()
{
	return this->area;
}

void Asteroid::PhysiqueManager::reset()
{
	this->player.reset();
	this->meteorManager.reset();
	this->powerUpManager.reset();
}