#pragma once
#include <SFML/System/Clock.hpp>
#include "Renderer.h"
#include "Window.h"

namespace Asteroid 
{
	enum class GameState
	{
		MAIN_MENU,
		RUNNING,
		PAUSED,
		HIGH_SCORE,
		GAME_OVER,
		QUIT,
	};

	class IGameManager {
	protected:
		sf::Clock clock;
		float deltaTime;
		GameState currentState;
		Renderer renderer;
		
		void updateDeltaTime();

	public:
		IGameManager();

		const float getDeltaTime() const;

		const GameState getState() const;
		
		virtual void update() = 0;
		
		virtual void init(Window& window, const char* configFile) = 0;
	};
}