#include "pch.h"
#include "MeteorConfig.h"

Asteroid::MeteorConfig::MeteorConfig()
{
	this->size = MeteorSize::SMALL;
	this->life = 0;
	this->score = 0;
	this->radius = 0;
}

Asteroid::MeteorConfig::MeteorConfig(MeteorSize size, int sizeDataScale, int minLife, int minScore, unsigned int maxRadius)
{
	this->size = size;

	int multiplicator = int(static_cast<MeteorSize>(size)) * sizeDataScale;
	this->life = multiplicator == 0? minLife: minLife * multiplicator;
	this->score = multiplicator == 0 ? minScore : minScore * multiplicator;
	
	switch (size)
	{
	case Asteroid::MeteorSize::SMALL:
		this->radius = maxRadius / 4;
		break;
	case Asteroid::MeteorSize::MEDIUM:
		this->radius = maxRadius / 2;
		break;
	case Asteroid::MeteorSize::BIG:
		this->radius = maxRadius;
		break;
	default:
		this->radius = maxRadius / 4;
		break;
	}
	
}

const Asteroid::MeteorSize Asteroid::MeteorConfig::getSize() const
{
	return this->size;
}

int& Asteroid::MeteorConfig::getLife()
{
	return this->life;
}

const int Asteroid::MeteorConfig::getScore() const
{
	return this->score;
}

const int Asteroid::MeteorConfig::getRadius() const
{
	return this->radius;
}
