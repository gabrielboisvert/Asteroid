#pragma once
#include "Player.h"
#include "MeteorManager.h"
#include "QuadTree.h"
#include "PowerUPManager.h"

namespace Asteroid
{
	class PhysiqueManager
	{
	private:
		Player player;
		MeteorManager meteorManager;
		QuadTree area;
		PowerUPManager powerUpManager;
	public:
		void init();

		void update();

		void checkCollision();

		bool collide(Sprite* sp, Meteor* meteor);

		Player& getPlayer();

		MeteorManager& getMeteorManager();

		PowerUPManager& getPowerUPManager();

		QuadTree& getQuad();

		void reset();
	};
}
