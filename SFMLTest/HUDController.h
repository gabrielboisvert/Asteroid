#pragma once
#include "Text.h"
#include "Sprite.h"
#include <SFML\System\Clock.hpp>

#include "Timer.h"

namespace Asteroid
{
	class HUDController
	{
	private:
		bool isInit;
		
		Sprite* lifeLeft[2];
		Sprite* shieldLeft[2];
		
		Text* sfMultiplicator;

		Text* sfScore;
		unsigned int scoreSec;
		float scoreTimer;

		Text* sfTime;
		float seconds;
		float minutes;
		std::string strTime;
		Timer timer;

		const std::string to_string_precision(const float value, const int digit) const;
	public:
		HUDController();

		void init();

		void updateTime();

		void updateScore();

		void updateBarShield();

		void updateLifeLeft();

		Sprite* getShieldLeft();

		Sprite* getShieldMax();

		void update();

		void pauseTimer();

		void reset();

		~HUDController();
	};
}

