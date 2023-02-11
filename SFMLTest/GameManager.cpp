#include "pch.h"
#include "GameManager.h"

Asteroid::GameManager Asteroid::GameManager::instance;

Asteroid::GameManager& Asteroid::GameManager::getInstance()
{
	return Asteroid::GameManager::instance;
}

Asteroid::GameManager::GameManager() {}

void Asteroid::GameManager::init(Window& window, const char* configFile)
{
	this->cfg = Config(configFile);
	
	this->renderer.init(&window);
	this->inputScanner.init(&window);
	this->scrollingManager.init();
	this->physiqueManager.init();
	this->hubManager.init();
	this->soundManager.init();
	this->highScoreManager.init();
	
	this->updateState();
}

Config& Asteroid::GameManager::getConfig()
{
	return this->cfg;
}

Asteroid::ScrollingManager& Asteroid::GameManager::getScrollManager()
{
	return this->scrollingManager;
}

Asteroid::InputScanner& Asteroid::GameManager::getInputScanner()
{
	return this->inputScanner;
}

Asteroid::Renderer& Asteroid::GameManager::getRenderer()
{
	return this->renderer;
}

Asteroid::PhysiqueManager& Asteroid::GameManager::getPhysiqueManager()
{
	return this->physiqueManager;
}

Asteroid::SoundManager& Asteroid::GameManager::getSoundManager()
{
	return this->soundManager;
}

Asteroid::HighScoreManager& Asteroid::GameManager::getHighScoreManager()
{
	return this->highScoreManager;
}

void Asteroid::GameManager::setState(GameState state)
{
	this->currentState = state;
	this->updateState();
}

void Asteroid::GameManager::update()
{
	this->updateDeltaTime();
	
	this->inputScanner.update();
	this->soundManager.update();
	
	if (this->currentState == GameState::MAIN_MENU || this->currentState == GameState::RUNNING || this->currentState == GameState::HIGH_SCORE)
	{
		this->scrollingManager.update();
		this->physiqueManager.update();
	}

	this->renderer.DrawSprites();
}

void Asteroid::GameManager::updateState()
{
	this->clock.restart();

	if (this->currentState == GameState::MAIN_MENU)
	{
		this->physiqueManager.reset();
		this->highScoreManager.reset();
	}
	else if (this->currentState == GameState::RUNNING) {}
	else if (this->currentState == GameState::PAUSED) 
	{
		this->physiqueManager.getPlayer().getHud().pauseTimer();
	}
	else if (this->currentState == GameState::HIGH_SCORE) {}
	else if (this->currentState == GameState::GAME_OVER)
	{
		GameManager::getInstance().getHighScoreManager().isNewHighScore(GameManager::getInstance().getPhysiqueManager().getPlayer().getScore());
		this->soundManager.playState();
	}
	else if (this->currentState == GameState::QUIT) {}

	this->hubManager.updateHud();
	this->inputScanner.updateAction();
}