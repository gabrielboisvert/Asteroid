#include "pch.h"
#include "InputScanner.h"
#include "GameManager.h"
#include <iostream>

Asteroid::InputScanner::InputScanner()
{
    this->window = nullptr;
}

bool Asteroid::InputScanner::init(Window* renderWindow)
{
	if (renderWindow == nullptr)
		return false;
	this->window = renderWindow;
	return true;
}

bool Asteroid::InputScanner::update()
{
    sf::Event event;
    while (this->window->pollEvent(event))
    {
        if (this->isQuitEvent(event)) {
            this->window->quit();
            GameManager::getInstance().setState(GameState::QUIT);
            break;
        }
        else if (this->isKeyBoardEvent(event))
        {
            if (GameManager::getInstance().getState() == GameState::MAIN_MENU && event.type == sf::Event::EventType::KeyPressed && event.key.code != sf::Keyboard::Escape)
            {
                GameManager::getInstance().setState(GameState::RUNNING);
                GameManager::getInstance().getSoundManager().playState();

                if (event.key.code == sf::Keyboard::P)
                    return true;
            }

            Asteroid::KeyBoardEvent keyEvent = KeyBoardEvent(event);
            std::map<Asteroid::KeyBoardEvent, Asteroid::InputScanner::KeyBoardAction>::iterator it = this->keyboardActions.find(event);
            if (it != this->keyboardActions.end())
                it->second();
        }
        else if (this->isMouseClickEvent(event))
        {
            Asteroid::MouseClickEvent clickEvent = MouseClickEvent(event);
            if (this->clickActions.find(clickEvent) != this->clickActions.end())
                this->clickActions[clickEvent](event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
        }
    }
	return true;
}

void Asteroid::InputScanner::updateAction()
{
    this->clearInputActions();

    GameState state = GameManager::getInstance().getState();

    if (state == GameState::MAIN_MENU) 
    {
        InputScanner::KeyBoardAction esc = [this]()
        {
            this->window->quit();
            GameManager::getInstance().setState(GameState::QUIT);
        };
        this->addKeyBoardAction(KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key::Escape, sf::Event::EventType::KeyPressed), esc);
        GameManager::getInstance().getPhysiqueManager().getPlayer().bindAction();
    }
    else if (state == GameState::RUNNING) 
    {
        InputScanner::KeyBoardAction esc = [this]() { GameManager::getInstance().setState(GameState::PAUSED); };
        
        this->addKeyBoardAction(KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key::Escape, sf::Event::EventType::KeyPressed), esc);
        this->addKeyBoardAction(KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key::P, sf::Event::EventType::KeyPressed), esc);

        GameManager::getInstance().getPhysiqueManager().getPlayer().bindAction();
    }
    else if (state == GameState::PAUSED) 
    {
        InputScanner::KeyBoardAction esc = [this]() { GameManager::getInstance().setState(GameState::RUNNING); };
        this->addKeyBoardAction(KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key::Escape, sf::Event::EventType::KeyPressed), esc);
        this->addKeyBoardAction(KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key::P, sf::Event::EventType::KeyPressed), esc);

        InputScanner::KeyBoardAction quit = [this]() { GameManager::getInstance().setState(GameState::MAIN_MENU); };
        this->addKeyBoardAction(KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key::Q, sf::Event::EventType::KeyPressed), quit);
    }
    else if (state == GameState::HIGH_SCORE)
    {
        InputScanner::KeyBoardAction esc = [this]() { GameManager::getInstance().setState(GameState::MAIN_MENU); };
        this->addKeyBoardAction(KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key::Escape, sf::Event::EventType::KeyPressed), esc);
        this->addKeyBoardAction(KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key::Enter, sf::Event::EventType::KeyPressed), esc);
    }
    else if (state  == GameState::GAME_OVER)
    {
        if (GameManager::getInstance().getHighScoreManager().getIsNewHighScore())
        {
            this->nameController.init(*this->window);
            GameManager::getInstance().getHighScoreManager().addNewScore(this->nameController.getName(),
                GameManager::getInstance().getPhysiqueManager().getPlayer().getScore());


            GameManager::getInstance().getPhysiqueManager().reset();
            GameManager::getInstance().setState(GameState::HIGH_SCORE);
        }
        else
        {
            InputScanner::KeyBoardAction quit = [this]() { GameManager::getInstance().setState(GameState::MAIN_MENU); };
            this->addKeyBoardAction(KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key::Q, sf::Event::EventType::KeyPressed), quit);
            this->addKeyBoardAction(KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key::Escape, sf::Event::EventType::KeyPressed), quit);
        }
    }

    else if (state == GameState::QUIT) {}
}

void Asteroid::InputScanner::addKeyBoardAction(KeyBoardEvent event, KeyBoardAction action)
{
    this->keyboardActions[event] = action;
}

void Asteroid::InputScanner::addMouseAction(MouseClickEvent event, MouseClickAction action)
{
	this->clickActions.insert(std::pair<MouseClickEvent, MouseClickAction>(event, action));
}

void Asteroid::InputScanner::clearInputActions()
{
	this->keyboardActions.clear();
}

bool Asteroid::InputScanner::isKeyBoardEvent(sf::Event& event)
{
	return event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::KeyReleased;
}

bool Asteroid::InputScanner::isMouseClickEvent(sf::Event& event)
{
	return event.type == sf::Event::EventType::MouseButtonPressed || event.type == sf::Event::EventType::MouseButtonReleased;
}

bool Asteroid::InputScanner::isQuitEvent(sf::Event& event)
{
	return event.type == sf::Event::EventType::Closed;
}