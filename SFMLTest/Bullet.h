#pragma once
#include "GameEntity.h"

namespace Asteroid
{
	class Bullet : public GameEntity
	{
	public:
		Bullet(Sprite* sprite);

		void move(float delta, float speed);

		~Bullet();
	};
}

