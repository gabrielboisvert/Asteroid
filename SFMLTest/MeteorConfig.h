#pragma once

namespace Asteroid
{
	enum class MeteorSize
	{
		SMALL = 0,
		MEDIUM = 1,
		BIG = 2,
		SIZE = 3
	};

	class MeteorConfig
	{
	private:
		MeteorSize size;
		int life;
		int score;
		unsigned int radius;
		
	public:
		MeteorConfig();

		MeteorConfig(MeteorSize size, int sizeDataScale, int minLife, int minScore, unsigned int maxRadius);

		const MeteorSize getSize() const;

		int& getLife();

		const int getScore() const;

		const int getRadius() const;
	};
}