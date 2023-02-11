#pragma once
#include "GameEntity.h"

namespace Asteroid
{
	class PowerUP : public GameEntity
	{
	public:
		PowerUP(Sprite* sprite);

		virtual void activate() = 0;

		~PowerUP();
	};
}