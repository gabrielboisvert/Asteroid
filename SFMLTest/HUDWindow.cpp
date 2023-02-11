#include "pch.h"
#include "HUDWindow.h"
#include "GameManager.h"

Asteroid::HUDWindow::HUDWindow(Sprite* sprite)
{
	this->image = sprite;
}

Asteroid::HUDButton* Asteroid::HUDWindow::addButton(Sprite* sprite, Text* text, std::function<void()>* action)
{
	if (this->image != nullptr)
		sprite->translate(this->image->getPosition().x, this->image->getPosition().y);
	
	HUDButton* button = new HUDButton(sprite, text, action);
	this->buttons.push_back(button);
	return button;
}

std::vector<Asteroid::HUDButton*>& Asteroid::HUDWindow::getButtons()
{
	return this->buttons;
}

Asteroid::Sprite* Asteroid::HUDWindow::getSprite()
{
	return this->image;
}

Asteroid::HUDWindow::~HUDWindow() 
{
	GameManager::getInstance().getRenderer().removeSprite(this->image);
	delete this->image;
	for (unsigned int i = 0; i < this->buttons.size(); i++)
		delete this->buttons[i];
	this->buttons.clear();
}