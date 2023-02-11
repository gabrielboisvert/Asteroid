#include "pch.h"
#include "Timer.h"

Timer::Timer() {
    this->started = false;
    this->paused = false;
    this->runTime = 0;
    this->clock.restart();
}

bool Timer::isPaused()
{
    return this->paused;
}

bool Timer::isStarted()
{
    return this->started;
}

void Timer::reset() 
{
    this->clock.restart();
    this->runTime = 0;
    this->paused = false;
    this->started = false;
}

void Timer::start() 
{
    if (this->paused) {
        this->clock.restart();
    }
    this->paused = false;
    this->started = true;
}

void Timer::pause() 
{
    if (!this->paused) {
        runTime += this->clock.getElapsedTime().asSeconds();
    }
    this->paused = true;
    this->started = false;
}

float Timer::GetElapsedSeconds() 
{
    if (!this->paused) {
        return runTime + this->clock.getElapsedTime().asSeconds();
    }
    return runTime;
}