#pragma once
#include "HUDButton.h"
#include <vector>

namespace Asteroid
{
	class HUDWindow 
	{
	private:
		Sprite* image;
		std::vector<HUDButton*> buttons;

	public:
		HUDWindow(Sprite* sprite);

		HUDButton* addButton(Sprite* sprite, Text* text, std::function<void()>* action);

		std::vector<HUDButton*>& getButtons();

		Sprite* getSprite();

		~HUDWindow();
	};
}