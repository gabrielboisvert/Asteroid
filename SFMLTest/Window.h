#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Sprite.h"

namespace Asteroid {
	class Window {
	private:
		sf::RenderWindow window;
		
	public:
		void init();

		sf::RenderWindow& getWindow();

		void quit();

		bool isOpen();

		bool pollEvent(sf::Event& event);

		void clear();

		void render(sf::Drawable& sprite, sf::Transform& transform);

		void render(sf::Drawable& sprite);

		void display();
	};
}