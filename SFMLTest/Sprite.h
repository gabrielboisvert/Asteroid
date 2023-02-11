#pragma once
#include <SFML/Graphics/Sprite.hpp>

namespace Asteroid 
{
	class Sprite : public sf::Sprite {
	private:
		sf::Transform transform;
		sf::FloatRect hitBox;
		float angle;
		void applyTransform();

	public:
		Sprite(const sf::Texture& texture, sf::Vector2i hitBox);

		sf::Transform& getTransform();

		const sf::FloatRect getGlobalBound() const;

		const sf::Vector2f getCenter() const;

		void setPosition(sf::Vector2f pos);

		sf::Vector2f getPosition();

		const sf::Vector2f getSize() const;

		const float getWidth() const;

		const float getHeight() const;

		const sf::Vector2f getCorner() const;

		void rotate(float angle);

		void scale(float x, float y);

		void translate(float x, float y);

		void resetTransform();
	
		typedef int layer;
	};
}