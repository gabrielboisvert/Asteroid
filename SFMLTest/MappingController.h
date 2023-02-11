#pragma once
#include <map>
#include <string>
#include "SFML/Window/Keyboard.hpp"


namespace Asteroid
{
	enum class Button
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		FIRE,
		SHIELD,
		FIRE_MODE
	};

	class MappingController
	{
	public:
		void init();
		sf::Keyboard::Key getButton(Button button);
		

	private:
		std::map<Button, sf::Keyboard::Key> buttons;

	};
}



