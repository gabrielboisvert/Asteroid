#include "pch.h"
#include "Player.h"
#include "GameManager.h"
#include <iostream>

Asteroid::Player::Player() 
{
	this->shieldSprite = nullptr;
	this->bulletTexture = nullptr;
	this->barHud = nullptr;
	this->fuelSprite = nullptr;
	this->fuelAnimationState = -1;

	this->life = 0;
	this->maxLife = 0;

	this->hitDommage = 0;
	this->fireDamage = 0;

	this->speed = 0;
	this->directionX = 0;
	this->directionY = 0;

	this->yDefaultPos = 0.0f;
	this->yMouvement = 15.0f;
	this->direction = -1;
	this->delay = 1.15f;

	this->score = 0;
	this->scoreMultiplicator = 1;
}

void Asteroid::Player::init()
{
	Config& cfg = GameManager::getInstance().getConfig();
	Renderer& renderer = GameManager::getInstance().getRenderer();

	sf::Texture* texture = renderer.loadTextureFile(*cfg.getValue("player", "player_ship"));
	this->sprite = renderer.createSprite(*texture, sf::Vector2i(0, 0), 4);

	sf::Texture* shieldTexture = renderer.loadTextureFile(*cfg.getValue("player", "player_shield"));
	this->shieldSprite = renderer.createSprite(*shieldTexture, sf::Vector2i(int(this->sprite->getPosition().x), int(this->sprite->getPosition().y)), 4);
	renderer.removeSprite(this->shieldSprite);

	this->bulletTexture = renderer.loadTextureFile(*cfg.getValue("player", "player_bullet"));

	this->barHud = renderer.loadTextureFile(*cfg.getValue("UI_Button", "bar"));

	Section* section = cfg.getSection("Fuel_animation");
	for (std::unordered_map<std::string, std::string>::iterator it = section->keyvalues.begin(); it != section->keyvalues.end(); it++)
		this->fuelTextures.push_back(renderer.loadTextureFile(it->second));
	this->fuelSprite = renderer.createSprite(*this->fuelTextures[0], sf::Vector2i(0, 0), 3);
	
	this->speed = std::stof(*cfg.getValue("player", "player_speed"));
	this->life = this->maxLife = std::stoi(*cfg.getValue("player", "player_life"));
	this->hitDommage = std::stoi(*cfg.getValue("player", "player_hit_damage"));

	this->fireDamage = std::stoi(*cfg.getValue("player", "player_firing_damage"));
	//Idle movement
	this->yDefaultPos = this->sprite->getPosition().y;
	this->speed = std::stof(*cfg.getValue("player", "player_speed"));

	this->controls.init();
	this->soundsEffect.init();

	this->timer[PlayerTimer::FIRE] = { sf::Time::Zero, std::stof(*cfg.getValue("player", "player_firing_rate")) };
	this->timer[PlayerTimer::SHIELD_UP] = { sf::Time::Zero, std::stof(*cfg.getValue("player", "player_shield_duration")) };
	this->timer[PlayerTimer::SHIELD_DOWN] = { sf::Time::Zero, std::stof(*cfg.getValue("player", "player_shield_cooldown")) };
	this->timer[PlayerTimer::FUEL_ANIMATION] = { sf::Time::Zero, std::stof(*cfg.getValue("player", "fuel_animation_delay")) };
}
void Asteroid::Player::bindAction()  //pouvoir lier une touche a une dirrection
{
	InputScanner& scanner = GameManager::getInstance().getInputScanner();
	
	InputScanner::KeyBoardAction upPress = [this]() //lorsque l'on presse la touche
	{
		this->directionY = -1;
	};
	InputScanner::KeyBoardAction upRelease = [this]() //lorsque l'on la release
	{
		if (this->directionY != 1)
			this->directionY = 0;
	};

	InputScanner::KeyBoardAction downPress = [this]() //lorsque l'on presse la touche
	{
		this->directionY = 1;
	};
	InputScanner::KeyBoardAction downRelease = [this]() //lorsque l'on la release
	{
		if (this->directionY != -1)
			this->directionY = 0;
	};
	InputScanner::KeyBoardAction leftPress = [this]() //lorsque l'on presse la touche
	{
		this->directionX = -1;
	};
	InputScanner::KeyBoardAction leftRelease = [this]() //lorsque l'on la release
	{
		if (this->directionX != 1)
			this->directionX = 0;
	};
	InputScanner::KeyBoardAction rightPress = [this]() //lorsque l'on presse la touche
	{
		this->directionX = 1;
	};
	InputScanner::KeyBoardAction rightRelease = [this]() //lorsque l'on la release
	{
		if (this->directionX != -1)
			this->directionX = 0;
	};

	InputScanner::KeyBoardAction shieldPress = [this]() //lorsque l'on presse la touche
	{
		if (this->playerStates[PlayerBool::SHIELDED] || !this->playerStates[PlayerBool::SHIELD_READY])
			return;

		GameManager::getInstance().getSoundManager().addSound(this->soundsEffect.getSound(PlayerSound::SHIELD_UP));
		this->playerStates[PlayerBool::SHIELDED] = true;
		this->timer[PlayerTimer::SHIELD_UP].first = sf::Time::Zero;
		
		GameManager::getInstance().getRenderer().addSprite(this->shieldSprite, 3);
	};

	InputScanner::KeyBoardAction fireMode = [this]() //lorsque l'on presse la touche
	{
		GameManager::getInstance().getSoundManager().addSound(this->soundsEffect.getSound(PlayerSound::SWITCH));
		this->playerStates[PlayerBool::FIRE_MODE] = !this->playerStates[PlayerBool::FIRE_MODE];
	};

	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::UP), sf::Event::EventType::KeyPressed), upPress);
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::UP), sf::Event::EventType::KeyReleased), upRelease);
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::DOWN), sf::Event::EventType::KeyPressed), downPress);
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::DOWN), sf::Event::EventType::KeyReleased), downRelease);
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::LEFT), sf::Event::EventType::KeyPressed), leftPress);
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::LEFT), sf::Event::EventType::KeyReleased), leftRelease);
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::RIGHT), sf::Event::EventType::KeyPressed), rightPress);
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::RIGHT), sf::Event::EventType::KeyReleased), rightRelease);
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::SHIELD), sf::Event::EventType::KeyPressed), shieldPress);
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::FIRE_MODE), sf::Event::EventType::KeyPressed), fireMode);
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::FIRE), sf::Event::EventType::KeyPressed), [this]() { this->shoot(); });
	scanner.addKeyBoardAction(KeyBoardEvent(this->controls.getButton(Button::FIRE), sf::Event::EventType::KeyReleased), [this]() { this->playerStates[PlayerBool::FIRING] = false; });
}

bool Asteroid::Player::getShield()
{
	return this->playerStates[PlayerBool::SHIELDED];
}

bool& Asteroid::Player::isShieldReady()
{
	return this->playerStates[PlayerBool::SHIELD_READY];
}

sf::Time& Asteroid::Player::getShieldCounter()
{
	return this->timer[PlayerTimer::SHIELD_UP].first;
}

float Asteroid::Player::getShieldMaxTime()
{
	return this->timer[PlayerTimer::SHIELD_UP].second;
}

sf::Time& Asteroid::Player::getShieldCoolDown()
{
	return this->timer[PlayerTimer::SHIELD_DOWN].first;
}

float Asteroid::Player::getShieldCoolDownMaxTime()
{
	return this->timer[PlayerTimer::SHIELD_DOWN].second;
}

sf::Texture* Asteroid::Player::getBar()
{
	return this->barHud;
}

std::vector<Asteroid::Bullet*>& Asteroid::Player::getBullets()
{
	return this->bullets;
}

int Asteroid::Player::getBulletDamage()
{
	return this->fireDamage;
}

int& Asteroid::Player::getHealth()
{
	return life;
}

int Asteroid::Player::getDommage()
{
	return hitDommage;
}

void Asteroid::Player::hit()
{
	if (this->playerStates[PlayerBool::SHIELDED])
		this->changeShield();
	else if (this->life > this->hitDommage)
	{
		this->life -= this->hitDommage;

		if (this->scoreMultiplicator > 1)
		{
			GameManager::getInstance().getSoundManager().addSound(this->soundsEffect.getSound(PlayerSound::MULTIPLIER_RESET));
			this->scoreMultiplicator = 1;
		}
		this->hud.updateLifeLeft();
		GameManager::getInstance().getSoundManager().addSound(this->soundsEffect.getSound(PlayerSound::HIT));
		
	}
	else
	{
		this->life = 0;
		this->hud.updateLifeLeft();
		GameManager::getInstance().getSoundManager().addSound(this->soundsEffect.getSound(PlayerSound::DEAD));
		GameManager::getInstance().setState(GameState::GAME_OVER);
	}
}

void Asteroid::Player::changeShield()
{
	GameManager::getInstance().getSoundManager().addSound(this->soundsEffect.getSound(PlayerSound::SHIELD_HIT));
	GameManager::getInstance().getSoundManager().addSound(this->soundsEffect.getSound(PlayerSound::SHIELD_DOWN));

	this->playerStates[PlayerBool::SHIELDED] = false;
	this->playerStates[PlayerBool::SHIELD_READY] = false;
	this->timer[PlayerTimer::SHIELD_DOWN].first = sf::Time::Zero;

	GameManager::getInstance().getRenderer().removeSprite(this->shieldSprite);
}

void  Asteroid::Player::move()
{
	float delta = GameManager::getInstance().getDeltaTime();
	sf::Vector2f screen = GameManager::getInstance().getRenderer().getWindow()->getWindow().getView().getSize();

	float actualSpeed = this->playerStates[PlayerBool::FIRE_MODE] ? this->speed / 2 : this->speed;
	
	this->translate((actualSpeed * delta) * this->directionX, (actualSpeed * delta) * this->directionY);
	sf::Vector2f newPos = this->sprite->getPosition();
	
	if (this->fuelSprite->getPosition().x <= 0 || newPos.x + this->sprite->getWidth() >= screen.x)
		this->translate((actualSpeed * delta) * -this->directionX, 0);
	
	if (newPos.y <= 0 || newPos.y + this->sprite->getHeight() >= screen.y)
		this->translate(0, (actualSpeed * delta) * -this->directionY);
	
	for (unsigned int i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i]->move(delta, this->speed * 1.5f);
		if (this->bullets[i]->getSprite()->getPosition().x > screen.x)
		{
			delete this->bullets[i];
			this->bullets.erase(std::find(this->bullets.begin(), this->bullets.end(), this->bullets[i]));
		}
	}

	this->fireTimer(delta);
	this->shieldTimer(delta);
	this->animateFuel(delta);

	this->hud.update();
}

void Asteroid::Player::shoot()
{
	if (!this->playerStates[PlayerBool::CAN_FIRE])
		return;

	GameManager::getInstance().getSoundManager().addSound(this->soundsEffect.getSound(PlayerSound::FIRE));
	Bullet* bullet = new Bullet(GameManager::getInstance().getRenderer().createSprite(*this->bulletTexture, sf::Vector2i(int(this->sprite->getCenter().x), int(this->sprite->getCenter().y)), 2));
	bullet->getSprite()->translate(0, -(bullet->getSprite()->getHeight() / 2));
	this->bullets.push_back(bullet);

	this->timer[PlayerTimer::FIRE].first = sf::Time::Zero;
	this->playerStates[PlayerBool::CAN_FIRE] = false;
	this->playerStates[PlayerBool::FIRING] = true;
}

void Asteroid::Player::animateFuel(float delta)
{
	this->timer[PlayerTimer::FUEL_ANIMATION].first += sf::milliseconds(sf::Int32(delta));;

	if (this->timer[PlayerTimer::FUEL_ANIMATION].first >= sf::seconds(this->timer[PlayerTimer::FUEL_ANIMATION].second))
	{
		this->timer[PlayerTimer::FUEL_ANIMATION].first = sf::Time::Zero;
		this->fuelAnimationState = ((int64_t(this->fuelAnimationState) + 1) % this->fuelTextures.size());
		this->fuelSprite->setTexture(*this->fuelTextures[this->fuelAnimationState]);
	}
}

void Asteroid::Player::shieldTimer(float delta)
{
	if (!this->playerStates[PlayerBool::SHIELDED] && !this->playerStates[PlayerBool::SHIELD_READY])
	{
		this->timer[PlayerTimer::SHIELD_DOWN].first += sf::milliseconds(sf::Int32(delta));
		if (this->timer[PlayerTimer::SHIELD_DOWN].first >= sf::seconds(this->timer[PlayerTimer::SHIELD_DOWN].second))
			this->playerStates[PlayerBool::SHIELD_READY] = true;
	}
	else if (this->playerStates[PlayerBool::SHIELDED])
	{
		this->timer[PlayerTimer::SHIELD_UP].first += sf::milliseconds(sf::Int32(delta));
		if (this->timer[PlayerTimer::SHIELD_UP].first >= sf::seconds(this->timer[PlayerTimer::SHIELD_UP].second))
		{
			GameManager::getInstance().getSoundManager().addSound(this->soundsEffect.getSound(PlayerSound::SHIELD_DOWN));
			this->playerStates[PlayerBool::SHIELDED] = false;
			this->playerStates[PlayerBool::SHIELD_READY] = false;
			this->timer[PlayerTimer::SHIELD_UP].first = sf::Time::Zero;
			this->timer[PlayerTimer::SHIELD_DOWN].first = sf::Time::Zero;
			GameManager::getInstance().getRenderer().removeSprite(this->shieldSprite);
		}
	}
}

void Asteroid::Player::fireTimer(float delta)
{
	if (this->playerStates[PlayerBool::CAN_FIRE] && this->playerStates[PlayerBool::FIRING])
		this->shoot();
	else if (this->playerStates[PlayerBool::CAN_FIRE])
		return;

	this->timer[PlayerTimer::FIRE].first += sf::milliseconds(sf::Int32(delta));
	if (this->timer[PlayerTimer::FIRE].first >= sf::seconds(this->timer[PlayerTimer::FIRE].second))
	{
		this->timer[PlayerTimer::FIRE].first = sf::Time::Zero;
		this->playerStates[PlayerBool::CAN_FIRE] = true;
	}
}

void Asteroid::Player::translate(float x, float y)
{
	this->sprite->translate(x, y);
	this->fuelSprite->translate(x, y);
	this->shieldSprite->translate(x, y);
}

void Asteroid::Player::setPosition(float x, float y)
{
	this->sprite->setPosition(sf::Vector2f(x, y));
	this->shieldSprite->setPosition(sf::Vector2f(this->sprite->getCenter().x, this->sprite->getCenter().y));
	this->shieldSprite->translate(-(this->shieldSprite->getWidth() / 2), -(this->shieldSprite->getHeight() / 2));
	
	this->fuelSprite->setPosition(sf::Vector2f(this->sprite->getPosition().x, this->sprite->getPosition().y + this->sprite->getHeight() / 2));
	this->fuelSprite->translate(-(this->fuelSprite->getWidth()), -(this->fuelSprite->getHeight() / 2));
}

int Asteroid::Player::getMaxLife()
{
	return this->maxLife;
}

Asteroid::Sprite* Asteroid::Player::getShieldSprite()
{
	return this->shieldSprite;
}

Asteroid::HUDController& Asteroid::Player::getHud()
{
	return this->hud;
}

Asteroid::SoundController& Asteroid::Player::getSoundsEffect()
{
	return this->soundsEffect;
}

unsigned int& Asteroid::Player::getScore()
{
	return this->score;
}

unsigned int& Asteroid::Player::getMultiplicator()
{
	return this->scoreMultiplicator;
}

void Asteroid::Player::idleMainMenu()
{
	float delta = GameManager::getInstance().getDeltaTime();
	this->animateFuel(delta);

	float currentPos = this->sprite->getPosition().y;

	float finalPos = this->yDefaultPos + (yMouvement * direction);
	if (direction == 1 && currentPos > finalPos)
	{
			direction = -1;
			this->mouvementCounter = sf::Time::Zero;
	}
	else if (direction == -1 && currentPos < finalPos)
	{
		direction = 1;
		this->mouvementCounter = sf::Time::Zero;
	}
	finalPos = this->yDefaultPos + (yMouvement * direction);
 
	this->timer[PlayerTimer::FUEL_ANIMATION].first += sf::milliseconds(sf::Int32(delta));

	float c = float(this->timer[PlayerTimer::FUEL_ANIMATION].first.asMilliseconds());
	float d = float(sf::seconds(this->delay).asMilliseconds());
	
	float timeLeft = d - c;

	this->translate(0, (finalPos / timeLeft) * direction);
}

void Asteroid::Player::reset()
{
	sf::Vector2f screenSize = GameManager::getInstance().getRenderer().getWindow()->getWindow().getView().getSize();

	this->setPosition(0, screenSize.y / 2.0f);
	this->translate(this->sprite->getWidth(), -(this->sprite->getHeight() / 2));

	//Idle mouvement
	this->yDefaultPos = this->sprite->getPosition().y;
	
	this->hud.reset();

	this->score = 0;
	this->life = this->maxLife = std::stoi(*GameManager::getInstance().getConfig().getValue("player", "player_life"));

	this->directionX = 0;
	this->directionY = 0;
	

	this->timer[PlayerTimer::FIRE].first = sf::Time::Zero;
	this->timer[PlayerTimer::SHIELD_UP].first = sf::Time::Zero;
	this->timer[PlayerTimer::SHIELD_DOWN].first = sf::Time::Zero;
	
	this->playerStates[PlayerBool::FIRE_MODE] = false;
	this->playerStates[PlayerBool::SHIELDED] = false;
	this->playerStates[PlayerBool::SHIELD_READY] = true;
	this->playerStates[PlayerBool::CAN_FIRE] = true;
	this->playerStates[PlayerBool::FIRING] = false;


	for (unsigned int i = 0; i < this->bullets.size(); i++)
		delete this->bullets[i];
	this->bullets.clear();
}

Asteroid::Player::~Player()
{
	Renderer& renderer = GameManager::getInstance().getRenderer();
	renderer.removeSprite(this->sprite);
	delete this->sprite;
	
	renderer.removeSprite(this->fuelSprite);
	delete this->fuelSprite;
}