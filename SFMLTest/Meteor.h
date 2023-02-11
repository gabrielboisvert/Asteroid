#pragma once
#include "GameEntity.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "MeteorConfig.h"

namespace Asteroid
{
	class Meteor : public GameEntity
	{
	private:
		MeteorConfig config;
		sf::CircleShape cercle;
		float selfRotation;
		float angleDirection;
		float speed; 
		
		void rotate(float delta);
		void translate(float x, float y);
	
	public:
		Meteor(sf::Texture* texture, MeteorConfig config, sf::Vector2i position);

		void initMouvement(float selfRotation, float angleDirection, float speed);

		void move();

		sf::CircleShape& getCercle();

		MeteorConfig& getConfig();

		~Meteor();
	};


}