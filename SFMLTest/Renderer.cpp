#include "pch.h"
#include "Renderer.h"
#include "GameManager.h"

Asteroid::Renderer::Renderer()
{
	this->window = nullptr;
	this->font = nullptr;
}

void Asteroid::Renderer::init(Asteroid::Window* window)
{
	if (window == nullptr)
		return;
	this->window = window;
	
	for (unsigned int i = 0; i < 4; i++)
		this->sprites.emplace_back();

	Config& cfg = GameManager::getInstance().getConfig();
	this->loadFont(*cfg.getValue("Fonts", "font3"));
}

Asteroid::Window* Asteroid::Renderer::getWindow() const
{
	return this->window;
}

Asteroid::Sprite* Asteroid::Renderer::createSprite(const sf::Texture& texture, sf::Vector2i hitBox, Sprite::layer zLayer)
{
	Sprite* sprite = new Sprite(texture, hitBox);

	if (this->sprites.size() <= zLayer) {
		std::list<Sprite*> list;
		this->sprites.push_back(list);
	}
	
	this->sprites[zLayer].push_back(sprite);

	return sprite;
}

void Asteroid::Renderer::addSprite(Sprite* sprite, Sprite::layer zLayer)
{
	if (this->sprites.size() <= zLayer) {
		std::list<Sprite*> list;
		this->sprites.push_back(list);
	}

	this->sprites[zLayer].push_back(sprite);
}

void Asteroid::Renderer::removeSprite(Sprite* sprite)
{
	for (unsigned int i = 0; i < this->sprites.size(); i++)
		this->sprites[i].remove(sprite);
}

sf::Texture* Asteroid::Renderer::loadTextureFile(const std::string& name)
{
	sf::Texture* texture = new sf::Texture;
	texture->loadFromFile(name);
	texture->setSmooth(true);

	this->textures.insert(std::pair<std::string, sf::Texture*>(name, texture));
	return this->textures[name];
}

sf::Texture* Asteroid::Renderer::getTexture(const std::string& name)
{
	if (this->textures.find(name) == this->textures.end())
		return nullptr;

	return this->textures[name];
}

void Asteroid::Renderer::loadFont(const std::string& path)
{
	this->font = new sf::Font;
	this->font->loadFromFile(path);
}

sf::Font* Asteroid::Renderer::getFont()
{
	return this->font;
}

Asteroid::Text* Asteroid::Renderer::createText(const std::string& str, const unsigned int size, sf::Vector2i hitBox, const sf::Color color)
{
	Text* text = new Text(*this->font, str, size, hitBox, color);
	this->texts.push_back(text);
	return text;
}

void Asteroid::Renderer::removeText(Text* text)
{
	std::vector<Text*>::iterator it = std::find(this->texts.begin(), this->texts.end(), text);
	this->texts.erase(it);
}

void Asteroid::Renderer::DrawSprites()
{
	for (unsigned int i = 0; i < this->sprites.size(); i++)
		for (std::list<Sprite*>::iterator it = this->sprites[i].begin(); it != this->sprites[i].end(); it++)
			this->window->render(**it, (*it)->getTransform());

	for (unsigned int i = 0; i < this->texts.size(); i++)
		this->window->render(*this->texts[i], this->texts[i]->getTransform());
}

void Asteroid::Renderer::clear()
{
	for (unsigned int i = 0; i < this->sprites.size(); i++)
		this->sprites[i].clear();
	this->sprites.clear();
}

void Asteroid::Renderer::freeTexture()
{
	if (this->textures.size() != 0)
	{
		for (std::map<std::string, sf::Texture*>::iterator it = this->textures.begin(); it != this->textures.end(); it++)
			delete it->second;
	}
}

Asteroid::Renderer::~Renderer() {}