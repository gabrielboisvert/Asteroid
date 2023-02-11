#pragma once
#include "IniParser.h"
#include <set>

namespace Asteroid
{
	class HighScoreManager
	{
	public:
		struct Compare
		{
			//To sort the multiset by value first than key if same value
			bool operator()(std::pair<std::string, unsigned int> a, std::pair<std::string, unsigned int> b) const
			{
				if (a.second == b.second)
					return a.first > b.first;
				return a.second > b.second;
			}
		};

		void init();

		bool& getIsNewHighScore();

		bool isNewHighScore(const unsigned int score);

		void addNewScore(const std::string name, const unsigned int score);

		void saveHighScore();

		void reset();

		//Multiset to accept multiple same name as high score
		std::multiset<std::pair<std::string, unsigned int>, Compare>& getHighScore();

	private:
		const static unsigned int MAX_HIGH_SCORE = 10;

		Config file;
		std::multiset<std::pair<std::string, unsigned int>, Compare> highScore;
		bool newHighScore;
	};
}