#include "pch.h"
#include "App.h"
#include "SFML/Window/Event.hpp"


Asteroid::App::App(const char* configFile)
{   
    this->window.init();
    this->gameManager.init(this->window, configFile);
}

void Asteroid::App::run()
{
    while (this->window.isOpen())
    {   this->window.clear();
        
        this->gameManager.update();
        if (this->gameManager.getState() == GameState::QUIT)
            break;
        
        this->window.display();
    }

    this->gameManager.getRenderer().freeTexture();
}