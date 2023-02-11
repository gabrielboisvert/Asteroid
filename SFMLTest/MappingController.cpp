#include "pch.h"
#include "MappingController.h"
#include "GameManager.h"


using namespace Asteroid;

void MappingController::init()
{
	Config& cfg = GameManager::getInstance().getConfig();
	this->buttons[Button::UP] = sf::Keyboard::Key(std::stoi(*cfg.getValue("controls", "UP")));
	this->buttons[Button::DOWN] = sf::Keyboard::Key(std::stoi(*cfg.getValue("controls", "DOWN")));
	this->buttons[Button::LEFT] = sf::Keyboard::Key(std::stoi(*cfg.getValue("controls", "LEFT")));
	this->buttons[Button::RIGHT] = sf::Keyboard::Key(std::stoi(*cfg.getValue("controls", "RIGHT")));
	this->buttons[Button::FIRE] = sf::Keyboard::Key(std::stoi(*cfg.getValue("controls", "FIRE")));
	this->buttons[Button::SHIELD] = sf::Keyboard::Key(std::stoi(*cfg.getValue("controls", "SHIELD")));
	this->buttons[Button::FIRE_MODE] = sf::Keyboard::Key(std::stoi(*cfg.getValue("controls", "FIRE_MODE")));
	
}


sf::Keyboard::Key MappingController::getButton(Button button)
{
	return this->buttons[button];
}