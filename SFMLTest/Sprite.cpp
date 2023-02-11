#include "pch.h"
#include "Sprite.h"

Asteroid::Sprite::Sprite(const sf::Texture& texture, sf::Vector2i hitBox)
{
	this->setTexture(texture);
	
	this->angle = 0;
	this->transform = sf::Transform::Identity;

	this->translate(float(hitBox.x), float(hitBox.y));
}

sf::Transform& Asteroid::Sprite::getTransform()
{
	return this->transform;
}

const sf::FloatRect Asteroid::Sprite::getGlobalBound() const
{
	return this->hitBox;
}

const sf::Vector2f Asteroid::Sprite::getCenter() const
{
	return sf::Vector2f(this->hitBox.left + (this->hitBox.width / 2), this->hitBox.top + (this->hitBox.height / 2));
}

sf::Vector2f Asteroid::Sprite::getPosition()
{
	return sf::Vector2f(this->hitBox.left, this->hitBox.top);
}

void Asteroid::Sprite::setPosition(sf::Vector2f pos)
{
	this->resetTransform();
	this->translate(pos.x, pos.y);
	this->rotate(this->angle);
}

const sf::Vector2f Asteroid::Sprite::getSize() const
{
	return sf::Vector2f(this->getLocalBounds().width, this->getLocalBounds().height);
}

const float Asteroid::Sprite::getWidth() const
{
	return this->hitBox.width;
}

const float Asteroid::Sprite::getHeight() const
{
	return this->hitBox.height;
}

const sf::Vector2f Asteroid::Sprite::getCorner() const
{
	return sf::Vector2f(this->hitBox.left + this->hitBox.width, this->hitBox.top + this->hitBox.height);
}

void Asteroid::Sprite::rotate(float angle)
{
	sf::Vector2f size = this->getSize();
	this->transform.rotate(angle, size.x / 2, size.y / 2);
	this->angle += angle;
	this->applyTransform();
}

void Asteroid::Sprite::scale(float x, float y)
{
	this->transform.scale(x, y);
	this->applyTransform();
}

void Asteroid::Sprite::translate(float x, float y)
{
	if (this->angle != 0)
	{
		float angle = this->angle;
		this->rotate(-this->angle);
		this->transform.translate(x, y);
		this->rotate(angle);
	}
	else
		this->transform.translate(x, y);

	this->applyTransform();
}

void Asteroid::Sprite::applyTransform()
{
	this->hitBox = this->transform.transformRect(sf::FloatRect(0, 0, this->getLocalBounds().width, this->getLocalBounds().height));
}

void Asteroid::Sprite::resetTransform()
{
	this->transform = sf::Transform::Identity;
}
