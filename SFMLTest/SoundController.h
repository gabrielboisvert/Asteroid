#pragma once
#include "SFML/Audio.hpp"
#include <map>

namespace Asteroid
{
	enum class PlayerSound
	{
		FIRE,
		SHIELD_UP,
		SHIELD_DOWN,
		SHIELD_HIT,
		HIT,
		DEAD,
		SWITCH,
		MULTIPLIER_RESET,
		POWER_UP
	};

	class SoundController
	{
	private:
		std::map<PlayerSound, sf::SoundBuffer*> soundEffect;

	public:
		void init();

		sf::SoundBuffer* getSound(const PlayerSound sound);

		~SoundController();
	};
}

