#pragma once
#include "SFML/Audio.hpp"
#include <vector>
#include <string>
#include <deque>
#include <map>

namespace Asteroid
{
	enum class SoundState
	{
		NEW_GAME,
		GAME_OVER,
		NEW_HI_SCORE
	};

	class SoundManager
	{
	private:
		std::vector<std::string>  playlist;
		int currentMusic = -1;
		sf::Music* music;

		std::deque<sf::Sound*> soundEffect;
		std::map<SoundState, sf::SoundBuffer*> soundState;
	public:
		SoundManager();

		void init();

		void update();

		void changeMusic();

		sf::SoundBuffer* createSound(const std::string& filename);

		void addSound(sf::SoundBuffer* sound);

		void clearSoundEffect();

		void playState();

		~SoundManager();
	};
}
