#include "pch.h"
#include "IGameManager.h"

Asteroid::IGameManager::IGameManager()
{
	this->deltaTime = 0;
	this->currentState = GameState::MAIN_MENU;
}

void Asteroid::IGameManager::updateDeltaTime()
{
	this->deltaTime = float(this->clock.restart().asMilliseconds());
}

const float Asteroid::IGameManager::getDeltaTime() const
{
	return this->deltaTime;
}

const Asteroid::GameState Asteroid::IGameManager::getState() const
{
	return this->currentState;
}