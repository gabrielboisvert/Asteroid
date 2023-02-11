#include "pch.h"
#include "HUDButton.h"
#include "GameManager.h"

Asteroid::HUDButton::HUDButton(Sprite* sprite, Text* text, std::function<void()>* action)
{
	this->image = sprite;
	this->action = action;
	this->text = text;
}

Asteroid::Sprite* Asteroid::HUDButton::getSprite()
{
	return this->image;
}

void Asteroid::HUDButton::click()
{
	if(this->action != nullptr)
		(*action)();
}

Asteroid::HUDButton::~HUDButton()
{
	Renderer& renderer = GameManager::getInstance().getRenderer();
	
	if (this->image != nullptr)
	{
		renderer.removeSprite(this->image);
		delete this->image;
	}
	
	if (this->text != nullptr)
	{
		renderer.removeText(this->text);
		delete this->text;
	}

	if (this->action != nullptr)
		delete this->action;
}
