#pragma once

#include "Sprite.h"

namespace Asteroid
{
	class GameEntity
	{
	protected:
		Sprite* sprite;

	public:
		GameEntity();
		Sprite* getSprite();
		void setSprite(Sprite* sprite);
		~GameEntity() {};
	};
}