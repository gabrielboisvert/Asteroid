#include "pch.h"
#include "PowerUPManager.h"
#include "GameManager.h"
#include "HealPowerUP.h"
#include "ShieldPowerUP.h"
#include "MultiPowerUP.h"

void Asteroid::PowerUPManager::init()
{
	Config& cfg = GameManager::getInstance().getConfig();
	Renderer& renderer = GameManager::getInstance().getRenderer();

	Section* section = cfg.getSection("powerups");
	for (std::unordered_multimap<std::string, std::string>::iterator it = section->keyvalues.begin(); it != section->keyvalues.end(); it++)
	{
		sf::Texture* texture = renderer.loadTextureFile(it->second);
		this->powerTexture.push_back(texture);
	}

	this->chanceOfSpawn = std::stoi(*cfg.getValue("powerups_val", "chance_of_spawn"));
	this->speed = std::stof(*cfg.getValue("powerups_val", "speed"));
}

std::vector<Asteroid::PowerUP*>& Asteroid::PowerUPManager::getPowerUP()
{
	return this->powerUPs;
}

void Asteroid::PowerUPManager::removePowerUP(PowerUP* powerUP)
{
	GameManager::getInstance().getSoundManager().addSound(GameManager::getInstance().getPhysiqueManager().getPlayer().getSoundsEffect().getSound(PlayerSound::POWER_UP));
	powerUP->activate();
	this->powerUPs.erase(std::find(this->powerUPs.begin(), this->powerUPs.end(), powerUP));
	delete powerUP;
}

void Asteroid::PowerUPManager::generate(sf::Vector2i pos)
{
	Renderer& renderer = GameManager::getInstance().getRenderer();
	if (!(rand() % this->chanceOfSpawn))
	{
		int type = rand() % PowerUPManager::SIZE;
		Sprite* sprite = renderer.createSprite(*this->powerTexture[type], pos, 4);
		
		PowerUP* powerUP = nullptr;
		switch (type)
		{
		case PowerUPManager::HP : powerUP = new HealPowerUP(sprite);
			break;
		case PowerUPManager::SHIELD: powerUP = new ShieldPowerUP(sprite);
			break;
		case PowerUPManager::POINTS: powerUP = new MultiPowerUP(sprite);
			break;
		default: return;
			break;
		}
		
		this->powerUPs.push_back(powerUP);
	}
}

void Asteroid::PowerUPManager::move()
{
	float delta = GameManager::getInstance().getDeltaTime();
	for (unsigned int i = 0; i < this->powerUPs.size(); i++)
	{
		PowerUP* powerUP = this->powerUPs[i];
		powerUP->getSprite()->translate(-(this->speed * delta), 0);
		if (powerUP->getSprite()->getPosition().x <= 0)
		{
			this->powerUPs.erase(std::find(this->powerUPs.begin(), this->powerUPs.end(), powerUP));
			delete powerUP;
		}
	}
}

void Asteroid::PowerUPManager::reset()
{
	for (unsigned int i = 0; i < this->powerUPs.size(); i++)
		delete this->powerUPs[i];
	this->powerUPs.clear();
}

Asteroid::PowerUPManager::~PowerUPManager()
{
	this->reset();
}
