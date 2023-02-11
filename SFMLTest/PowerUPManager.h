#pragma once
#include "PowerUP.h"
#include <vector>

namespace Asteroid
{
	

	class PowerUPManager
	{
	private:	
		enum
		{
			HP,
			SHIELD,
			POINTS,
			SIZE
		};

		std::vector<PowerUP*> powerUPs;
		std::vector<sf::Texture*> powerTexture;
		int chanceOfSpawn;
		float speed;

	public:
	
		void init();

		std::vector<PowerUP*>& getPowerUP();

		void removePowerUP(PowerUP* powerUP);

		void generate(sf::Vector2i pos);

		void move();

		void reset();

		~PowerUPManager();
	};
}