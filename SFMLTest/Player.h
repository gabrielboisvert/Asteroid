#pragma once
#include "GameEntity.h"
#include "IniParser.h"
#include "Renderer.h"
#include "MappingController.h"
#include "SoundController.h"
#include "HUDController.h"
#include "Bullet.h"
#include <map>

namespace Asteroid
{
	enum class PlayerTimer
	{
		FIRE,
		SHIELD_UP,
		SHIELD_DOWN,
		FUEL_ANIMATION
	};

	enum class PlayerBool
	{
		FIRE_MODE,
		SHIELDED,
		SHIELD_READY,
		CAN_FIRE,
		FIRING,
	};
	
	class Player : public GameEntity
	{
	private:
		Sprite* shieldSprite;
		
		sf::Texture* bulletTexture;
		std::vector<Bullet*> bullets;
		
		sf::Texture* barHud;
		
		std::vector<sf::Texture*> fuelTextures;
		Sprite* fuelSprite;
		int fuelAnimationState;

		std::map<PlayerTimer, std::pair<sf::Time, float>> timer;
		std::map<PlayerBool, bool> playerStates;

		int life;
		int maxLife;
		
		int hitDommage;
		int fireDamage;
		
		float speed;
		int directionY;
		int directionX;

		unsigned int score;
		unsigned int scoreMultiplicator;

		//Idle movement
		float yDefaultPos = 0.0f;
		float yMouvement = 15.0f;
		int direction = -1;
		float delay = 1.15f;
		sf::Time mouvementCounter = sf::Time::Zero;

		MappingController controls;
		SoundController soundsEffect;
		HUDController hud;
		
	public:
		Player();

		void init();

		void bindAction();

		void animateFuel(float delta);

		void idleMainMenu();

		void translate(float x, float y);

		void setPosition(float x, float y);

		int getMaxLife();

		Sprite* getShieldSprite();

		HUDController& getHud();

		SoundController& getSoundsEffect();

		unsigned int& getScore();

		unsigned int& getMultiplicator();

		void move();

		void shoot();

		void reset();

		bool getShield();

		bool& isShieldReady();

		sf::Time& getShieldCounter();

		float getShieldMaxTime();

		sf::Time& getShieldCoolDown();
		
		float getShieldCoolDownMaxTime();

		sf::Texture* getBar();

		std::vector<Bullet*>& getBullets();

		int getBulletDamage();

		void changeShield();

		int& getHealth();

		void hit();

		int getDommage();

		void shieldTimer(float delta);

		void fireTimer(float delta);

		~Player();
	};
}