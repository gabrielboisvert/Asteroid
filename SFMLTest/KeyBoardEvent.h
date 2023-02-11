#pragma once

#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Event.hpp"

namespace Asteroid 
{
	class KeyBoardEvent
	{
	private:
		sf::Keyboard::Key keyCode;
		sf::Event::EventType event;

	public:
		KeyBoardEvent(sf::Keyboard::Key key, sf::Event::EventType type);
		KeyBoardEvent(sf::Event& event);

		const bool operator<(const KeyBoardEvent& event) const;
	};
}