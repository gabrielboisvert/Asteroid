#pragma once
#include <SFML\System\Clock.hpp>
class Timer
{
private:
    sf::Clock clock;
    float runTime;
    bool paused;
    bool started;
public:
    Timer();

    bool isPaused();

    bool isStarted();

    void reset();

    void start();

    void pause();

    float GetElapsedSeconds();
};

