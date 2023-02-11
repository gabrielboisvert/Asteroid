#include "pch.h"
#include "HighScoreManager.h"
#include "GameManager.h"

#include <iostream>
#include <fstream>

void Asteroid::HighScoreManager::init()
{
	this->file = Config("assets/highScore.txt");
	Section* names = this->file.getSection("Names");
	for (std::unordered_map<std::string, std::string>::iterator it = names->keyvalues.begin(); it != names->keyvalues.end(); it++)
		this->highScore.insert({ it->first, std::stoi(it->second) });
}

bool& Asteroid::HighScoreManager::getIsNewHighScore()
{
	return this->newHighScore;
}

bool Asteroid::HighScoreManager::isNewHighScore(const unsigned int score)
{
	if (this->highScore.size() < HighScoreManager::MAX_HIGH_SCORE)
	{
		this->newHighScore = true;
		return true;
	}

	for (std::multiset<std::pair<std::string, unsigned int>, Asteroid::HighScoreManager::Compare>::iterator it = this->highScore.begin(); it != this->highScore.end(); it++)
		if (score > it->second)
		{
			this->newHighScore = true;
			return true;
		}

	this->newHighScore = false;
	return false;
}

void Asteroid::HighScoreManager::addNewScore(const std::string name, const unsigned int score)
{
	this->highScore.insert({ name, score});
	if (this->highScore.size() > HighScoreManager::MAX_HIGH_SCORE)
		this->highScore.erase(std::prev(this->highScore.end()));

	this->saveHighScore();
}

void Asteroid::HighScoreManager::saveHighScore()
{
	std::ofstream file;
	file.open("assets/highScore.txt");
	file << "[Names]\n";
	for (std::multiset<std::pair<std::string, unsigned int>>::iterator it = this->highScore.begin(); it != this->highScore.end(); it++)
		file << it->first << " = " << it->second << "\n";
	file.close();
}

std::multiset<std::pair<std::string, unsigned int>, Asteroid::HighScoreManager::Compare>& Asteroid::HighScoreManager::getHighScore()
{
	return this->highScore;
}

void Asteroid::HighScoreManager::reset()
{
	this->newHighScore = false;
}