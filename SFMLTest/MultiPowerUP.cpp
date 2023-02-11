#include "pch.h"
#include "MultiPowerUP.h"
#include "GameManager.h"

Asteroid::MultiPowerUP::MultiPowerUP(Sprite* sprite) : PowerUP(sprite){}

void Asteroid::MultiPowerUP::activate()
{
	GameManager::getInstance().getPhysiqueManager().getPlayer().getMultiplicator()++;
}
