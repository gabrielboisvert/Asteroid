#pragma once
#include "HUDWindow.h"

namespace Asteroid
{
	class HUDManager
	{
	private:
		std::vector<sf::Texture*> buttons;
		std::vector<HUDWindow*> hubWindows;
		void initMainMenuButton();
		void initRunningButton();
		void initPauseButton();
		void initHighScore();
		void initGameOverButton();
	public:
		void init();

		void updateHud();

		void clear();
	};
}