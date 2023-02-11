#include "pch.h"
#include "ScrollingManager.h"
#include "GameManager.h"
#include <stdlib.h>

void Asteroid::ScrollingManager::init()
{
	if (this->hasBeenInit)
		return;

	srand(unsigned int(time(NULL)));

	Config& cfg = GameManager::getInstance().getConfig();
	Renderer& render = GameManager::getInstance().getRenderer();
	const sf::Vector2f screenSize = render.getWindow()->getWindow().getView().getSize();

	this->initBackground(cfg, render, screenSize);
	this->initObject(cfg, render, screenSize);

	this->layerScale[VERY_FAR] = std::stof(*cfg.getValue("parallax_veryfar_params", "scale"));
	this->layerScale[FAR] = std::stof(*cfg.getValue("parallax_far_params", "scale"));
	this->layerScale[NEAR] = std::stof(*cfg.getValue("parallax_near_params", "scale"));

	this->layerSpeed[VERY_FAR] = std::stof(*cfg.getValue("parallax_veryfar_params", "speed"));
	this->layerSpeed[FAR] = std::stof(*cfg.getValue("parallax_far_params", "speed"));
	this->layerSpeed[NEAR] = std::stof(*cfg.getValue("parallax_near_params", "speed"));

	this->spawnTimerMin = std::stoi(*cfg.getValue("parallax_spawn_timers", "min"));
	this->spawnTimerMax = std::stoi(*cfg.getValue("parallax_spawn_timers", "max"));
	this->direction = -1;
	
	this->spawnCounter = sf::Time::Zero;
	this->hasBeenInit = true;
}

void Asteroid::ScrollingManager::initBackground(Config& cfg, Asteroid::Renderer& render, const sf::Vector2f& screenSize)
{
	sf::Texture* texture = render.loadTextureFile(*cfg.getValue("Backgrounds", "background_image"));

	this->background[0] = render.createSprite(*texture, sf::Vector2i(0, 0), 0);
	this->background[0]->scale(1, screenSize.y / this->background[0]->getHeight());

	this->background[1] = render.createSprite(*texture, sf::Vector2i(int(this->background[0]->getCorner().x), 0), 0);
	this->background[1]->scale(1, screenSize.y / this->background[1]->getHeight());
}

void Asteroid::ScrollingManager::initObject(Config& cfg, Renderer& render, const sf::Vector2f& screenSize)
{
	unsigned int i = 0;
	Section* section = cfg.getSection("background_parallax_objects");
	for (std::unordered_map<std::string, std::string>::iterator it = section->keyvalues.begin(); it != section->keyvalues.end(); it++, i++)
	{
		sf::Texture* texture = render.loadTextureFile(it->second);
		this->parallaxObject.emplace_back();
		this->parallaxObject[i].sprite = new Sprite(*texture, sf::Vector2i(0, 0));
	}
}

void Asteroid::ScrollingManager::setDirection(int direction)
{
	this->direction = direction;
}

void Asteroid::ScrollingManager::update()
{
	const float& delta = GameManager::getInstance().getDeltaTime();
	this->spawnCounter += (sf::milliseconds(sf::Int32(delta)));
	if (this->spawnCounter >= sf::seconds(float(this->nextSpawnTimer)))
		this->spawnObject();

	this->scrollBackground(delta);
	this->scrollObject(delta);
}

void Asteroid::ScrollingManager::spawnObject()
{
	this->spawnCounter = sf::Time::Zero;
	this->nextSpawnTimer = this->spawnTimerMin + rand() % ((this->spawnTimerMax + 1) - this->spawnTimerMin);
	if (this->direction == 0)
		return;

	sf::Vector2f screenSize = GameManager::getInstance().getRenderer().getWindow()->getWindow().getView().getSize();
	screenSize.y /= 8;
	screenSize.y *= 6;
	for (unsigned int i = 0; i < SIZE; i++)
	{
		Renderer& render = GameManager::getInstance().getRenderer();
		ParallaxObject* obj = nullptr;
		do
		{
			obj = &this->parallaxObject[rand() % this->parallaxObject.size()];
			if (!obj->available)
				continue;
			
			obj->sprite->resetTransform();
			
			float value = (rand() % int(screenSize.x / 2)) + screenSize.x;
			
			float yMin = (screenSize.y / 3) * i;
			float yMax = (screenSize.y / 3) + yMin;
			float yValue = yMin + rand() % int((yMax + 1) - yMin);
			
			
			obj->sprite->translate(value, yValue);

			obj->sprite->scale(this->layerScale[i] / obj->sprite->getScale().x, this->layerScale[i] / obj->sprite->getScale().y);
			obj->available = false;
			obj->layer = i;

			render.addSprite(obj->sprite, i);

			break;
		} while (true);
	}
}

void Asteroid::ScrollingManager::scrollBackground(const float& delta)
{
	for (unsigned int i = 0; i < 2; i++)
	{
		float speed = ((this->layerSpeed[0] / 8) * delta);
		this->background[i]->translate(((this->layerSpeed[0] / 8) * delta) * this->direction, 0);
		if (this->background[i]->getCorner().x <= 0)
			this->switchBackground();
	}
	
}

void Asteroid::ScrollingManager::switchBackground()
{
	if (this->background[0]->getPosition().x < this->background[1]->getPosition().x)
		this->background[0]->translate(this->background[0]->getWidth() * 2, 0);
	else
		this->background[1]->translate(this->background[1]->getWidth() * 2, 0);
}

void Asteroid::ScrollingManager::scrollObject(const float& delta)
{
	for (unsigned int i = 0; i < this->parallaxObject.size(); i++)
	{
		ParallaxObject& obj = this->parallaxObject[i];
		if (!obj.available)
		{
			float speed = ((this->layerSpeed[obj.layer] * delta));
			obj.sprite->translate(((this->layerSpeed[obj.layer] * delta) * direction), 0);
			if (obj.sprite->getCorner().x <= 0)
			{
				obj.available = true;
				GameManager::getInstance().getRenderer().removeSprite(obj.sprite);
			}
		}
	}
}

Asteroid::ScrollingManager::~ScrollingManager()
{
	for (int i = 0; i < 2; i++)
		delete this->background[i];

	for (unsigned int i = 0; i < this->parallaxObject.size(); i++)
		delete this->parallaxObject[i].sprite;
}