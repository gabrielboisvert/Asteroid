#include "pch.h"
#include "SoundController.h"
#include "GameManager.h"

void Asteroid::SoundController::init()
{
	Config& cfg = GameManager::getInstance().getConfig();
	SoundManager& soundManager = GameManager::getInstance().getSoundManager();

	this->soundEffect[PlayerSound::FIRE] = soundManager.createSound(*cfg.getValue("player", "player_fire_sfx"));
	this->soundEffect[PlayerSound::SHIELD_UP] = soundManager.createSound(*cfg.getValue("player", "player_shield_up_sfx"));
	this->soundEffect[PlayerSound::SHIELD_DOWN] = soundManager.createSound(*cfg.getValue("player", "player_shield_down_sfx"));
	this->soundEffect[PlayerSound::SHIELD_HIT] = soundManager.createSound(*cfg.getValue("player", "player_shield_hit_sfx"));
	this->soundEffect[PlayerSound::HIT] = soundManager.createSound(*cfg.getValue("player", "player_hit_sfx"));
	this->soundEffect[PlayerSound::DEAD] = soundManager.createSound(*cfg.getValue("player", "player_dead_sfx"));
	this->soundEffect[PlayerSound::SWITCH] = soundManager.createSound(*cfg.getValue("player", "player_switch_sfx"));
	this->soundEffect[PlayerSound::MULTIPLIER_RESET] = soundManager.createSound(*cfg.getValue("soundEffects", "jingle_multiplier_reset_sfx"));
	this->soundEffect[PlayerSound::POWER_UP] = soundManager.createSound(*cfg.getValue("player", "powerup_hit_sfx"));
}

sf::SoundBuffer* Asteroid::SoundController::getSound(const PlayerSound sound)
{
	return this->soundEffect[sound];
}

Asteroid::SoundController::~SoundController()
{
	for (std::map<PlayerSound, sf::SoundBuffer*>::iterator it = this->soundEffect.begin(); it != this->soundEffect.end(); it++)
		delete it->second;
	this->soundEffect.clear();
}