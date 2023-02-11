#pragma once
#include "IniParser.h"
#include "Renderer.h"

namespace Asteroid
{
	class ScrollingManager
	{
	private:
		enum
		{
			VERY_FAR,
			FAR,
			NEAR,
			SIZE
		};
		struct ParallaxObject
		{
			Sprite* sprite = nullptr;
			bool available = true;
			unsigned int layer = 0;
		};

		Sprite* background[2];
		std::vector<ParallaxObject> parallaxObject;

		float layerSpeed[SIZE];
		float layerScale[SIZE];

		unsigned int spawnTimerMin;
		unsigned int spawnTimerMax;
		int direction;

		sf::Time spawnCounter;
		unsigned int nextSpawnTimer;

		bool hasBeenInit = false;
	public:
		void init();

		void initBackground(Config& cfg, Asteroid::Renderer& render, const sf::Vector2f& screenSize);
		
		void initObject(Config& cfg, Renderer& render, const sf::Vector2f& screenSize);
		
		void setDirection(int direction);

		void update();

		void scrollObject(const float& delta);

		void scrollBackground(const float& delta);

		void spawnObject();

		void switchBackground();

		~ScrollingManager();
	};
}