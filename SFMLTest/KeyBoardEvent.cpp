#include "pch.h"
#include "KeyBoardEvent.h"


Asteroid::KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key key, sf::Event::EventType type)
{
	this->event = type;
	this->keyCode = key;
}

Asteroid::KeyBoardEvent::KeyBoardEvent(sf::Event& event)
{
	this->keyCode = event.key.code;
	this->event = event.type;
}

const bool Asteroid::KeyBoardEvent::operator<(const KeyBoardEvent& event) const
{
	if (this->keyCode == event.keyCode)
		return this->event > event.event;
	return this->keyCode > event.keyCode;
}



