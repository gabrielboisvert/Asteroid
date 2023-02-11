#include "pch.h"
#include "Window.h"


void Asteroid::Window::init()
{
	this->window.create(sf::VideoMode(1200, 800), "Asteroid");
	this->window.setVerticalSyncEnabled(true);
}

sf::RenderWindow& Asteroid::Window::getWindow()
{
	return this->window;
}

void Asteroid::Window::quit()
{
	this->window.close();
}

bool Asteroid::Window::isOpen()
{
	return this->window.isOpen();
}

bool Asteroid::Window::pollEvent(sf::Event& event)
{
	return this->window.pollEvent(event);
}

void Asteroid::Window::clear()
{
	this->window.clear();
}

void Asteroid::Window::render(sf::Drawable& sprite, sf::Transform& transform)
{
	this->window.draw(sprite, transform);
}

void Asteroid::Window::render(sf::Drawable& sprite)
{
	this->window.draw(sprite);
}

void Asteroid::Window::display()
{
	this->window.display();
}