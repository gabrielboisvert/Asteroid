#pragma once
#include "Window.h"
#include "GameManager.h"

namespace Asteroid {
	class App {
	private:
		Window window;
		GameManager& gameManager = GameManager::getInstance();
	public:
		App(const char* configFile);
		void run();
	};
}