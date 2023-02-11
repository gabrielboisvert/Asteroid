#include "pch.h"
#include "HUDController.h"
#include "GameManager.h"
#include <sstream>

Asteroid::HUDController::HUDController()
{
    this->isInit = false;

    this->lifeLeft[0] = nullptr;
    this->lifeLeft[1] = nullptr;

    this->shieldLeft[0] = nullptr;
    this->shieldLeft[1] = nullptr;
    
    this->sfMultiplicator = nullptr;
    this->sfScore = nullptr;
    this->scoreSec = 0;
    this->scoreTimer = 0;

    this->sfTime = nullptr;
    this->seconds = 0;
    this->minutes = 0;
}

void Asteroid::HUDController::init()
{
    Renderer& renderer = GameManager::getInstance().getRenderer();
    sf::Vector2f screenSize = renderer.getWindow()->getWindow().getView().getSize();

    sf::Texture* texture = GameManager::getInstance().getPhysiqueManager().getPlayer().getBar();
    for (unsigned int i = 0; i < 2; i++)
    {
        this->lifeLeft[i] = renderer.createSprite(*texture, sf::Vector2i(50, int(screenSize.y - 100)), 5);
        if (i == 1)
            this->lifeLeft[i]->setColor(sf::Color::Green);
        this->lifeLeft[i]->scale((screenSize.x / 6) / this->lifeLeft[i]->getWidth(), 25 / this->lifeLeft[i]->getHeight());
        this->lifeLeft[i]->translate(0, -this->lifeLeft[i]->getHeight() * 1.5f);
    }

    for (unsigned int i = 0; i < 2; i++)
    {
        this->shieldLeft[i] = renderer.createSprite(*texture, sf::Vector2i(50, int(this->lifeLeft[i]->getPosition().y + 40)), 5);
        if (i == 1)
            this->shieldLeft[i]->setColor(sf::Color::Blue);
        this->shieldLeft[i]->scale((screenSize.x / 6) / this->shieldLeft[i]->getWidth(), 25 / this->shieldLeft[i]->getHeight());
    }

    this->scoreSec = std::stoi(*GameManager::getInstance().getConfig().getValue("player", "player_score_sec"));
    this->sfTime = renderer.createText("0 : 00", 30, sf::Vector2i(int(screenSize.x / 2), 0), sf::Color::White);
    this->sfTime->translate(-(this->sfTime->getWidth() / 2), this->sfTime->getHeight());
    
    this->sfScore = renderer.createText("Score: 0", 30, sf::Vector2i(0, 0), sf::Color::White);
    this->sfScore->translate(this->sfScore->getWidth() / 4, this->sfScore->getHeight());
    
    this->sfMultiplicator = renderer.createText("1x", 30, sf::Vector2i(int(this->sfScore->getPosition().x), int(this->sfScore->getCorner().y)), sf::Color::White);
    this->sfMultiplicator->translate(0, this->sfMultiplicator->getHeight() / 2);

    this->timer.reset();
    this->timer.start();
    this->isInit = true;
}

void Asteroid::HUDController::pauseTimer()
{
    this->timer.pause();
}

void Asteroid::HUDController::updateTime()
{
    if (this->timer.isPaused())
        this->timer.start();

    this->seconds = this->timer.GetElapsedSeconds() - 60 * this->minutes;
    if (this->seconds > 60)
        this->minutes++;

    if (this->seconds < 10)
        this->strTime = std::to_string(int(this->minutes)) + ":" + "0" + this->to_string_precision(this->seconds, 3);
    else
        this->strTime = std::to_string(int(this->minutes)) + ":" + this->to_string_precision(this->seconds, 3);

    this->sfTime->setString(this->strTime);
}

void Asteroid::HUDController::updateScore()
{   
    Player& player = GameManager::getInstance().getPhysiqueManager().getPlayer();
    
    if (this->timer.GetElapsedSeconds() - (1 * this->scoreTimer) > 1)
    {
        this->scoreTimer++;
        player.getScore() += this->scoreSec * player.getMultiplicator();
    }

    this->sfScore->setString("Score: " + std::to_string(player.getScore()));
    this->sfMultiplicator->setString(std::to_string(player.getMultiplicator()) + "x");
}

void Asteroid::HUDController::updateBarShield()
{
    Player& player = GameManager::getInstance().getPhysiqueManager().getPlayer();
    
    if (player.getShield())
    {
        float timeMax = player.getShieldMaxTime();
        float timeLeft = (sf::seconds(timeMax) - player.getShieldCounter()).asSeconds();
        float finalSize = (timeLeft * this->shieldLeft[0]->getWidth()) / timeMax;

        this->shieldLeft[1]->setScale(finalSize / this->shieldLeft[1]->getWidth(), 1);
    }
    else
    {
        if (player.isShieldReady())
        {
            this->shieldLeft[1]->setColor(sf::Color::Blue);
            return;
        }

        float timeMax = player.getShieldCoolDownMaxTime();
        float timeLeft = (sf::seconds(timeMax) - player.getShieldCoolDown()).asSeconds();
        float finalSize = ((timeLeft * this->shieldLeft[0]->getWidth()) / timeMax) - this->shieldLeft[0]->getWidth();

        this->shieldLeft[1]->setScale((finalSize / this->shieldLeft[1]->getWidth()) * -1, 1);

        if (this->shieldLeft[1]->getGlobalBounds().width != this->shieldLeft[0]->getGlobalBounds().width)
            this->shieldLeft[1]->setColor(sf::Color::Magenta);
    }
}

void Asteroid::HUDController::updateLifeLeft()
{
    Player& player = GameManager::getInstance().getPhysiqueManager().getPlayer();
    int maxLife = player.getMaxLife();
    int currentLife = player.getHealth();
    float finalSize = (currentLife * this->lifeLeft[0]->getWidth()) / maxLife;
    this->lifeLeft[1]->setScale(finalSize / this->lifeLeft[1]->getWidth(), 1);
    
    if (this->lifeLeft[0]->getGlobalBounds().width != this->lifeLeft[1]->getGlobalBounds().width)
        this->lifeLeft[1]->setColor(sf::Color::Red);
    else
        this->lifeLeft[1]->setColor(sf::Color::Green);
}

Asteroid::Sprite* Asteroid::HUDController::getShieldLeft()
{
    return this->shieldLeft[1];
}

Asteroid::Sprite* Asteroid::HUDController::getShieldMax()
{
    return this->shieldLeft[0];
}

const std::string Asteroid::HUDController::to_string_precision(const float value, const int digit) const
{
    std::ostringstream out;
    out.precision(digit);
    out << std::fixed << value;
    return out.str();
}

void Asteroid::HUDController::update()
{
    if (!this->isInit)
        this->init();

    this->updateTime();
    this->updateScore();
    this->updateBarShield();
}

void Asteroid::HUDController::reset()
{   
    Renderer& renderer = GameManager::getInstance().getRenderer();
    if (this->lifeLeft[0] != nullptr)
    {
        renderer.removeSprite(this->lifeLeft[0]);
        renderer.removeSprite(this->lifeLeft[1]);
        delete this->lifeLeft[0];
        delete this->lifeLeft[1];
        this->lifeLeft[0] = nullptr;
        this->lifeLeft[1] = nullptr;
    }
    
    if (this->shieldLeft[0] != nullptr)
    {
        renderer.removeSprite(this->shieldLeft[0]);
        renderer.removeSprite(this->shieldLeft[1]);
        delete this->shieldLeft[0];
        delete this->shieldLeft[1];
        this->shieldLeft[0] = nullptr;
        this->shieldLeft[1] = nullptr;
    }
    
    if (this->sfTime != nullptr)
    {
        renderer.removeText(this->sfTime);
        delete this->sfTime;
        this->sfTime = nullptr;
    }

    if (this->sfMultiplicator != nullptr)
    {
        renderer.removeText(this->sfMultiplicator);
        delete this->sfMultiplicator;
        this->sfMultiplicator = nullptr;
    }

    if (this->sfScore != nullptr)
    {
        renderer.removeText(this->sfScore);
        delete this->sfScore;
        this->sfScore = nullptr;
    }
    this->scoreTimer = 0;

    this->seconds = 0;
    this->minutes = 0;
    this->isInit = false;
}

Asteroid::HUDController::~HUDController()
{
    this->reset();
}