#pragma once
#include <SFML/Graphics/Text.hpp>

namespace Asteroid
{
	class Text : public sf::Text
	{
	private:
		sf::Transform transform;
		sf::FloatRect hitBox;

		void applyTransform();

	public:
		Text(const sf::Font& font, const std::string& str, const unsigned int size, sf::Vector2i hitBox, const sf::Color color);

		sf::Transform& getTransform();

		const sf::FloatRect getGlobalBound() const;

		const sf::Vector2f getPosition() const;

		const sf::Vector2f getSize() const;

		const float getWidth() const;

		const float getHeight() const;

		const sf::Vector2f getCorner() const;

		void rotate(float angle);

		void scale(float x, float y);

		void translate(float x, float y);

		void resetTransform();
	};
}