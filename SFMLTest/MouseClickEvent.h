#pragma once
#include "SFML/Window/Mouse.hpp"
#include "SFML/Window/Event.hpp"

namespace Asteroid 
{
	class MouseClickEvent 
	{
	private:
		sf::Mouse::Button button;
		sf::Event::EventType event;

	public:
		MouseClickEvent(sf::Mouse::Button button, sf::Event::EventType type);
		
		MouseClickEvent(sf::Event& event);

		bool operator<(const MouseClickEvent& mouseClickEvent) const;
	};
}