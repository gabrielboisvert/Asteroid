#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Sprite.h"
#include "Window.h"
#include <list>
#include <map>
#include <string>
#include "Text.h"
#include "IniParser.h"


namespace Asteroid 
{
	class Renderer {
	private:
		std::map<std::string, sf::Texture*> textures;
		std::vector<std::list<Sprite*>> sprites;
		std::vector<Text*> texts;
		Window* window;
		sf::Font* font;
	public:
		Renderer();

		void init(Window* window);
		Window* getWindow() const;

		Sprite* createSprite(const sf::Texture& texture, sf::Vector2i hitBox, Sprite::layer zLayer);
		void addSprite(Sprite* sprite, Sprite::layer zLayer = 0);
		void removeSprite(Sprite* sprite);
		
		sf::Texture* loadTextureFile(const std::string& name);
		sf::Texture* getTexture(const std::string& name);

		void loadFont(const std::string& path);
		sf::Font* getFont();
		Text* createText(const std::string& str, const unsigned int size, sf::Vector2i hitBox, const sf::Color color);
		void removeText(Text* text);

		void DrawSprites();

		void clear();
		void freeTexture();

		~Renderer();
	};
}