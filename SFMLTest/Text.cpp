#include "pch.h"
#include "Text.h"
#include "GameManager.h"

Asteroid::Text::Text(const sf::Font& font, const std::string& str, const unsigned int size, sf::Vector2i hitBox, const sf::Color color)
{
	this->setFont(font);
	this->setString(str);
	this->setCharacterSize(size);
	this->setFillColor(color);

	this->transform = sf::Transform::Identity;
	this->translate(float(hitBox.x), float(hitBox.y));
}

sf::Transform& Asteroid::Text::getTransform()
{
	return this->transform;
}

const sf::FloatRect Asteroid::Text::getGlobalBound() const
{
	return this->hitBox;
}

const sf::Vector2f Asteroid::Text::getPosition() const
{
	return sf::Vector2f(this->hitBox.left, this->hitBox.top);
}

const sf::Vector2f Asteroid::Text::getSize() const
{
	return sf::Vector2f(this->hitBox.width, this->hitBox.height);
}

const float Asteroid::Text::getWidth() const
{
	return this->hitBox.width;
}

const float Asteroid::Text::getHeight() const
{
	return this->hitBox.height;
}

const sf::Vector2f Asteroid::Text::getCorner() const
{
	return sf::Vector2f(this->hitBox.left + this->hitBox.width, this->hitBox.top + this->hitBox.height);
}

void Asteroid::Text::rotate(float angle)
{
	this->transform.rotate(angle);
	this->applyTransform();
}

void Asteroid::Text::scale(float x, float y)
{
	this->transform.scale(x, y);
	this->applyTransform();
}

void Asteroid::Text::translate(float x, float y)
{
	this->transform.translate(x, y);
	this->applyTransform();
}

void Asteroid::Text::applyTransform()
{
	this->hitBox = this->transform.transformRect(sf::FloatRect(0, 0, this->getLocalBounds().width, this->getGlobalBounds().height));
}

void Asteroid::Text::resetTransform()
{
	this->transform = sf::Transform::Identity;
}