#pragma once
#include "PowerUP.h"

namespace Asteroid
{
	class ShieldPowerUP : public PowerUP
	{
	private:
	public:
		ShieldPowerUP(Sprite* sprite);

		void activate() override;
	};
}