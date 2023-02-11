#pragma once
#include "Sprite.h"
#include <functional>
#include "Text.h"

namespace Asteroid
{
	class HUDButton 
	{
	private:
		Sprite* image;
		Text* text;
		std::function<void()>* action;
	
	public:
		HUDButton(Sprite* sprite, Text* text, std::function<void()>* action);

		Sprite* getSprite();

		void click();

		~HUDButton();
	};
}