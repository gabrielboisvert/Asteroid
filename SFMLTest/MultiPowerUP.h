#pragma once
#include "PowerUP.h"

namespace Asteroid
{
	class MultiPowerUP : public PowerUP
	{
	private:
	public:
		MultiPowerUP(Sprite* sprite);

		void activate() override;
	};
}

