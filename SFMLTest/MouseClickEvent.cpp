#include "pch.h"
#include "MouseClickEvent.h"


Asteroid::MouseClickEvent::MouseClickEvent(sf::Mouse::Button button, sf::Event::EventType type)
{
	this->button = button;
	this->event = type;
}

Asteroid::MouseClickEvent::MouseClickEvent(sf::Event& event)
{
	this->button = event.mouseButton.button;
	this->event = event.type;
}

bool Asteroid::MouseClickEvent::operator<(const MouseClickEvent& mouseClickEvent) const
{
	if (this->event > mouseClickEvent.event)
		return true;
	return false;
}