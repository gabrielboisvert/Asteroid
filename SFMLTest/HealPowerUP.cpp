#include "pch.h"
#include "HealPowerUP.h"
#include "GameManager.h"

Asteroid::HealPowerUP::HealPowerUP(Sprite* sprite) : PowerUP(sprite) {}

void Asteroid::HealPowerUP::activate()
{
	Player& player = GameManager::getInstance().getPhysiqueManager().getPlayer();
	player.getHealth() = player.getMaxLife();
	player.getHud().updateLifeLeft();
}
