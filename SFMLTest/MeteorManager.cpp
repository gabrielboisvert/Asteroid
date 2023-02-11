#include "pch.h"
#include "MeteorManager.h"
#include "GameManager.h"

Asteroid::MeteorManager::MeteorManager()
{
	srand(unsigned int(time(nullptr)));

	this->texture = nullptr;
	this->dataScale = 0;
	this->minLife = 0;
	this->minScore = 0;
	this->maxRadius = 0;
	this->maxSelfRotate = 0;
	this->minSpeed = 0;
	this->maxSpeed = 0;

	this->spawnMin = 0;
	this->spawnMax = 0;
	this->counter = sf::Time::Zero;
	this->nextSpawnTime = sf::Time::Zero;
}

void Asteroid::MeteorManager::init()
{
	Config& cfg = GameManager::getInstance().getConfig();
	Renderer& renderer = GameManager::getInstance().getRenderer();
	SoundManager& soundManager = GameManager::getInstance().getSoundManager();

	this->dataScale = std::stoi(*cfg.getValue("asteroids", "asteroid_scale"));
	this->minLife = std::stoi(*cfg.getValue("asteroids", "asteroid_life"));
	this->minScore = std::stoi(*cfg.getValue("asteroids", "asteroid_minScore"));
	this->maxRadius = std::stoi(*cfg.getValue("asteroids", "asteroid_circle_radius"));

	this->maxSelfRotate = std::stof(*cfg.getValue("asteroids", "asteroid_maxSelfRotate"));
	this->minSpeed = std::stof(*cfg.getValue("asteroids", "asteroid_minSpeed"));
	this->maxSpeed = std::stof(*cfg.getValue("asteroids", "asteroid_maxSpeed"));

	this->spawnMin = std::stoi(*cfg.getValue("asteroids", "spawnMin"));
	this->spawnMax = std::stoi(*cfg.getValue("asteroids", "spawnMax"));

	this->texture = renderer.loadTextureFile(*cfg.getValue("asteroids", "asteroid"));

	this->soundEffect[AsteroidSound::BROKEN] = soundManager.createSound(*cfg.getValue("asteroids", "asteroid_broken_sfx"));
	this->soundEffect[AsteroidSound::TOUCHED] = soundManager.createSound(*cfg.getValue("asteroids", "asteroid_touched_sfx"));
}

std::vector<Asteroid::Meteor*>& Asteroid::MeteorManager::getMeteors()
{
	return this->meteors;
}

void Asteroid::MeteorManager::move()
{
	this->counter += sf::milliseconds(sf::Int32(GameManager::getInstance().getDeltaTime()));
	if (this->counter >= this->nextSpawnTime)
		this->spawn();

	for (unsigned int i = 0; i < this->meteors.size(); i++)
	{
		Meteor* meteor = this->meteors[i];
		meteor->move();
		if (meteor->getSprite()->getCorner().x < 0)
		{	
			GameManager::getInstance().getPhysiqueManager().getQuad().removeMeteor(this->meteors[i]);
			this->meteors.erase(std::find(this->meteors.begin(), this->meteors.end(), meteor));
			delete meteor;
		}
	}
}

void Asteroid::MeteorManager::hit(Meteor* meteor)
{
	SoundManager& soundManager = GameManager::getInstance().getSoundManager();

	meteor->getConfig().getLife() -= GameManager::getInstance().getPhysiqueManager().getPlayer().getBulletDamage();
	if (meteor->getConfig().getLife() <= 0)
	{
		GameManager::getInstance().getPhysiqueManager().getPlayer().getScore() += meteor->getConfig().getScore() * GameManager::getInstance().getPhysiqueManager().getPlayer().getMultiplicator();
		soundManager.addSound(this->soundEffect[AsteroidSound::BROKEN]);
		this->split(meteor);
	}
	else
		soundManager.addSound(this->soundEffect[AsteroidSound::TOUCHED]);
}

void Asteroid::MeteorManager::spawn()
{
	this->counter = sf::Time::Zero;
	this->nextSpawnTime = sf::seconds(float(this->spawnMin + rand() % ((this->spawnMax + 1) - this->spawnMin)));

	sf::Vector2f screenSize = GameManager::getInstance().getRenderer().getWindow()->getWindow().getView().getSize();

	int size = rand() % int(static_cast<MeteorSize>(MeteorSize::SIZE));
	MeteorConfig cfg = MeteorConfig(static_cast<MeteorSize>(size), this->dataScale, this->minLife, this->minScore, this->maxRadius);
	sf::Vector2i pos = sf::Vector2i((rand() % int((screenSize.x / 2)) + int(screenSize.x)), rand() % int(screenSize.y));
	
	Meteor* meteor = new Meteor(this->texture, cfg, pos);

	float selfAngle = float(rand() % (int(this->maxSelfRotate * 100) * 2) + (-int(this->maxSelfRotate * 100)));
	selfAngle = selfAngle == 0 ? 0 : selfAngle / 100.0f;

	const sf::Vector2f playerPos = GameManager::getInstance().getPhysiqueManager().getPlayer().getSprite()->getCenter();;
	float direction = atan2(playerPos.y - meteor->getSprite()->getCenter().y, 
							playerPos.x - meteor->getSprite()->getCenter().x) * (180 / 3.1415f);

	float min = this->minSpeed * 100;
	float max = this->maxSpeed * 100;

	if (static_cast<MeteorSize>(size) == MeteorSize::SMALL)
	{
		min *= 3;
		max *= 3;
	}
	else
	{
		min *= 1.5f;
		max *= 1.5f;
	}
	float speed = (min + rand() % int((max + 1) - min)) / 100.0f;

	meteor->initMouvement(selfAngle, direction, speed);

	this->meteors.push_back(meteor);
}

void Asteroid::MeteorManager::split(Meteor* meteor)
{
	if (meteor->getConfig().getSize() != MeteorSize::SMALL)
	{
		MeteorSize size = MeteorSize(int(static_cast<MeteorSize>(meteor->getConfig().getSize())) - 1);
		MeteorConfig cfg = MeteorConfig(static_cast<MeteorSize>(size), this->dataScale, this->minLife, this->minScore, this->maxRadius);
		sf::Vector2i pos = sf::Vector2i(meteor->getSprite()->getCenter());

		for (unsigned int i = 0; i < 2; i++)
		{
			Meteor* newMeteor = new Meteor(this->texture, cfg, pos);
			newMeteor->getSprite()->translate(-(newMeteor->getSprite()->getWidth() / 2), -(newMeteor->getSprite()->getHeight() / 2));

			float selfAngle = float(rand() % (int(this->maxSelfRotate * 100) * 2) + (-int(this->maxSelfRotate * 100)));
			selfAngle = selfAngle == 0 ? 0 : selfAngle / 100.0f;

			float direction = float(150 + rand() % ((210 + 1) - 150));

			float min = this->minSpeed * 100;
			float max = this->maxSpeed * 100;
			
			if (size == MeteorSize::SMALL)
			{
				min *= 3;
				max *= 3;
			}
			else
			{
				min *= 1.5f;
				max *= 1.5f;
			}
			
			float speed = (min + rand() % int((max + 1) - min)) / 100.0f;
			newMeteor->initMouvement(selfAngle, -direction, speed);
			this->meteors.push_back(newMeteor);
		}
	}
	this->meteors.erase(std::find(this->meteors.begin(), this->meteors.end(), meteor));
	
	GameManager::getInstance().getPhysiqueManager().getPowerUPManager().generate(sf::Vector2i(meteor->getSprite()->getCenter()));

	GameManager::getInstance().getPhysiqueManager().getQuad().removeMeteor(meteor);
	delete meteor;
}

void Asteroid::MeteorManager::destroy(Meteor* meteor)
{
	GameManager::getInstance().getSoundManager().addSound(this->soundEffect[AsteroidSound::BROKEN]);
	this->meteors.erase(std::find(this->meteors.begin(), this->meteors.end(), meteor));
	delete meteor;
}

void Asteroid::MeteorManager::reset()
{
	for (unsigned int i = 0; i < this->meteors.size(); i++)
		delete this->meteors[i];
	this->meteors.clear();

	this->counter = sf::Time::Zero;
	this->nextSpawnTime = sf::Time::Zero;
}

Asteroid::MeteorManager::~MeteorManager()
{
	for (unsigned int i = 0; i < this->meteors.size(); i++)
		delete this->meteors[i];
	this->meteors.clear();

	delete this->soundEffect[AsteroidSound::BROKEN];
	delete this->soundEffect[AsteroidSound::TOUCHED];
}