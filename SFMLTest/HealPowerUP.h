#pragma once
#include "PowerUP.h"

namespace Asteroid
{
	class HealPowerUP : public PowerUP
	{
	public:

		HealPowerUP(Sprite* sprite);

		void activate() override;
	};
}