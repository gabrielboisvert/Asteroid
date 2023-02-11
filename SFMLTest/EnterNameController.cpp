#include "pch.h"
#include "EnterNameController.h"
#include "SFML/Window/Event.hpp"
#include "GameManager.h"

void Asteroid::EnterNameController::init(Window& window)
{
    this->window = &window;
    Renderer& renderer = GameManager::getInstance().getRenderer();
    sf::Vector2f screenSize = renderer.getWindow()->getWindow().getView().getSize();
    this->name = "";
    this->text = renderer.createText("Enter name: ", int(screenSize.y / 20), sf::Vector2i(int(screenSize.x / 2), int(screenSize.y / 2)), sf::Color::White);
    this->text->move(-(this->text->getWidth()), -(this->text->getHeight() / 2));

    sf::Event event;
    this->window->pollEvent(event);
    do
    {   
        this->window->clear();
        GameManager::getInstance().getRenderer().DrawSprites();
        this->window->display();

        while (this->window->getWindow().pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed) {
                this->window->quit();
                GameManager::getInstance().setState(GameState::QUIT);
                break;
            }

            if (event.key.code == sf::Keyboard::Enter && !this->name.empty())
            {
                renderer.removeText(this->text);
                delete this->text;
                this->text = nullptr;
                return;
            }
            else if (event.key.code == sf::Keyboard::BackSpace && !this->name.empty())
                this->name.pop_back();
            else if (event.type == sf::Event::TextEntered && this->name.size() < EnterNameController::MAX_LETTER && event.key.code != sf::Keyboard::Num1 && event.key.code != sf::Keyboard::N)
                this->name += static_cast<char>(event.text.unicode);
        }

        this->text->setString("Enter name: " + this->name);
        
    }while (this->window->isOpen());
}

const std::string Asteroid::EnterNameController::getName() const
{
	return this->name;
}

Asteroid::EnterNameController::~EnterNameController()
{
    if (this->text != nullptr)
    {
        GameManager::getInstance().getRenderer().removeText(this->text);
        delete this->text;
    }
}
