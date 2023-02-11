#pragma once
#include <vector>
#include "Meteor.h"

#define MAX_OBJS 2
#define MAX_LEVELS 5

namespace Asteroid
{
	class QuadTree
	{
	private:
		int level;
		std::vector<Meteor*> object;
		sf::FloatRect bounds;
		
		QuadTree* nodes[4];
	public:
		QuadTree();

		QuadTree(int level, sf::FloatRect bounds);

		void clear();

		void split();

		int getIndex(sf::FloatRect objPosition);

		void insert(Meteor* meteor);

		void removeMeteor(Meteor* meteor);

		std::vector<Meteor*>& retrieve(std::vector<Meteor*>& currentObject, sf::FloatRect bound);
	};
}