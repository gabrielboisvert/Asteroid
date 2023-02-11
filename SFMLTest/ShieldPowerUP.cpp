#include "pch.h"
#include "ShieldPowerUP.h"
#include "GameManager.h"

Asteroid::ShieldPowerUP::ShieldPowerUP(Sprite* sprite) : PowerUP(sprite) {}

void Asteroid::ShieldPowerUP::activate()
{
	Player& player = GameManager::getInstance().getPhysiqueManager().getPlayer();
	player.getShieldCounter() = sf::Time::Zero;
	player.getShieldCoolDown() = sf::seconds(float(player.getShieldCoolDownMaxTime()));
	player.getHud().getShieldLeft()->setScale(1, 1);
}
