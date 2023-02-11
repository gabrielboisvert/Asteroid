#include "pch.h"
#include "HUDManager.h"
#include "GameManager.h"

void Asteroid::HUDManager::init()
{
	Config& cfg = GameManager::getInstance().getConfig();
	Renderer& renderer = GameManager::getInstance().getRenderer();
	renderer.loadTextureFile(*cfg.getValue("UI_Button", "highScore"));

	Asteroid::InputScanner::MouseClickAction clickListener = [this](sf::Mouse::Button button, int x, int y)
	{
		for (unsigned int i = 0; i < this->hubWindows.size(); i++)
		{
			HUDWindow* window = this->hubWindows[i];
			for (unsigned int j = 0; j < window->getButtons().size(); j++)
			{
				HUDButton* button = window->getButtons()[j];
				if (button->getSprite() != nullptr)
					if (button->getSprite()->getGlobalBound().contains(float(x), float(y)))
					{
						button->click();
						return;
					}
			}
		}
	};
	GameManager::getInstance().getInputScanner().addMouseAction(MouseClickEvent(sf::Mouse::Button::Left, sf::Event::EventType::MouseButtonPressed), clickListener);
}

void Asteroid::HUDManager::updateHud()
{
	this->clear();

	const GameState& state = GameManager::getInstance().getState();
	switch (state)
	{
	case GameState::MAIN_MENU:	this->initMainMenuButton();
								break;

	case GameState::RUNNING:	this->initRunningButton();
								break;

	case GameState::PAUSED:	this->initPauseButton();
							break;

	case GameState::HIGH_SCORE:	this->initHighScore();
		break;

	case GameState::GAME_OVER:	this->initGameOverButton();
		break;

	default: this->initMainMenuButton();
			 break;
	}
}

void Asteroid::HUDManager::initMainMenuButton()
{
	Renderer& renderer = GameManager::getInstance().getRenderer();
	sf::Vector2f screenSize = renderer.getWindow()->getWindow().getView().getSize();
	HUDWindow* background = new HUDWindow(nullptr);
	
	Text* title = renderer.createText("Asteroid", unsigned int(screenSize.y / 20), sf::Vector2i(int(screenSize.x / 2), int(screenSize.y / 10)), sf::Color::White);
	title->translate(-(title->getWidth() / 2), 0);
	background->addButton(nullptr, title, nullptr);

	Text* actionText = renderer.createText("Press any button to start", unsigned int(screenSize.y / 32), sf::Vector2i(int(screenSize.x / 2), int(screenSize.y)), sf::Color::White);
	actionText->translate(-(actionText->getWidth() / 2), -(screenSize.y / 5));
	background->addButton(nullptr, actionText, nullptr);

	sf::Texture* icon = renderer.getTexture(*GameManager::getInstance().getConfig().getValue("UI_Button", "highScore"));
	Sprite* highScore = renderer.createSprite(*icon, sf::Vector2i(int(actionText->getPosition().x), int(actionText->getPosition().y)), 3);
	highScore->translate(actionText->getWidth() + (actionText->getWidth() / 4), -(actionText->getHeight() / 2));
	highScore->scale(50 / highScore->getWidth(), 50 / highScore->getHeight());
	
	std::function<void()>* clickHighScore = new std::function<void()>;
	*clickHighScore = []()
	{
		GameManager::getInstance().setState(GameState::HIGH_SCORE);
	};

	background->addButton(highScore, nullptr, clickHighScore);


	this->hubWindows.push_back(background);
}

void Asteroid::HUDManager::initRunningButton() {}

void Asteroid::HUDManager::initPauseButton()
{
	Renderer& renderer = GameManager::getInstance().getRenderer();
	sf::Vector2f screenSize = renderer.getWindow()->getWindow().getView().getSize();
	HUDWindow* background = new HUDWindow(nullptr);

	Text* title = renderer.createText("- Paused -", unsigned int(screenSize.y / 10), sf::Vector2i(int(screenSize.x / 2), int(screenSize.y / 3)), sf::Color::White);
	title->translate(-(title->getWidth() / 2), -(title->getHeight() / 2));
	background->addButton(nullptr, title, nullptr);

	this->hubWindows.push_back(background);
}

void Asteroid::HUDManager::initHighScore()
{
	Renderer& renderer = GameManager::getInstance().getRenderer();
	sf::Vector2f screenSize = renderer.getWindow()->getWindow().getView().getSize();
	HUDWindow* background = new HUDWindow(nullptr);

	Text* title = renderer.createText("- High scores -", unsigned int(screenSize.y / 10), sf::Vector2i(int(screenSize.x / 2), 0), sf::Color::White);
	title->translate(-(title->getWidth() / 2), (title->getHeight() / 1.5f));
	background->addButton(nullptr, title, nullptr);

	unsigned int yPos = int(title->getPosition().y + (title->getHeight() + (title->getHeight() / 2)));
	unsigned int yPadding = int(title->getHeight() / 2);

	unsigned int space = 0;
	std::multiset<std::pair<std::string, unsigned int>, Asteroid::HighScoreManager::Compare>& highScore = GameManager::getInstance().getHighScoreManager().getHighScore();
	for (std::multiset<std::pair<std::string, unsigned int>>::iterator it = highScore.begin(); it != highScore.end(); it++, space++)
	{
		sf::Vector2i pos(int(screenSize.x / 2), yPos + (yPadding * space));
		Text* nameScore = renderer.createText(std::to_string(space + 1) + ". " + it->first + " - " + std::to_string(it->second), unsigned int(screenSize.y / 25), pos, sf::Color::Green);
		nameScore->translate(-(nameScore->getWidth() / 2), (title->getHeight() / 4) * space);
		background->addButton(nullptr, nameScore, nullptr);
	}

	this->hubWindows.push_back(background);
}

void Asteroid::HUDManager::initGameOverButton()
{
	Renderer& renderer = GameManager::getInstance().getRenderer();
	sf::Vector2f screenSize = renderer.getWindow()->getWindow().getView().getSize();
	HUDWindow* background = new HUDWindow(nullptr);

	Text* title = renderer.createText("- Game over -", unsigned int(screenSize.y / 10), sf::Vector2i(int(screenSize.x / 2), int(screenSize.y / 4)), sf::Color::White);
	title->translate(-(title->getWidth() / 2), -(title->getHeight() / 2));
	background->addButton(nullptr, title, nullptr);

	if (GameManager::getInstance().getHighScoreManager().getIsNewHighScore())
	{
		Text* highScore = renderer.createText("New High Score", unsigned int(screenSize.y / 20), sf::Vector2i(int(screenSize.x / 2), int(screenSize.y / 2)), sf::Color::Green);
		highScore->translate(-(highScore->getWidth() / 2), -(highScore->getHeight() * 2.5f));
		background->addButton(nullptr, highScore, nullptr);
	}
	
	this->hubWindows.push_back(background);
}

void Asteroid::HUDManager::clear()
{
	for (unsigned int i = 0; i < this->hubWindows.size(); i++)
		delete this->hubWindows[i];
	this->hubWindows.clear();
}