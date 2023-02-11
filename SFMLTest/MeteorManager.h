#pragma once
#include "Meteor.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Audio.hpp"
#include <map>

namespace Asteroid
{
	enum class AsteroidSound
	{
		BROKEN,
		TOUCHED
	};

	class MeteorManager
	{
	private:
		sf::Texture* texture;
		std::vector<Meteor*> meteors;
		
		int dataScale;
		int minLife;
		int minScore;
		unsigned int maxRadius;
		float maxSelfRotate;
		float minSpeed;
		float maxSpeed;

		int spawnMin;
		int spawnMax;
		sf::Time counter;
		sf::Time nextSpawnTime;

		std::map<AsteroidSound, sf::SoundBuffer*> soundEffect;

	public:
		MeteorManager();

		void init();

		std::vector<Meteor*>& getMeteors();

		void move();

		void hit(Meteor* meteor);

		void spawn();

		void split(Meteor* meteor);

		void reset();

		void destroy(Meteor* meteor);

		~MeteorManager();
	};
}