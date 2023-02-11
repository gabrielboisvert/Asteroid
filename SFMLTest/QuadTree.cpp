#include "pch.h"
#include "QuadTree.h"

Asteroid::QuadTree::QuadTree()
{
	this->level = 0;
	for (unsigned int i = 0; i < 4; i++)
		this->nodes[i] = nullptr;
}

Asteroid::QuadTree::QuadTree(int level, sf::FloatRect bounds)
{
	this->level = level;
	this->bounds = bounds;

	for (unsigned int i = 0; i < 4; i++)
		this->nodes[i] = nullptr;
}

void Asteroid::QuadTree::clear()
{
	this->object.clear();

	for (unsigned int i = 0; i < 4; i++)
		if (this->nodes[i] != nullptr)
		{
			this->nodes[i]->clear();
			this->nodes[i] = nullptr;
		}
}

void Asteroid::QuadTree::split()
{
	float subWidth = (this->bounds.width / 2);
	float subHeight = (this->bounds.height / 2);
	float x = this->bounds.left;
	float y = this->bounds.top;

	this->nodes[0] = new QuadTree(level + 1, sf::FloatRect(x + subWidth, y, subWidth, subHeight));
	this->nodes[1] = new QuadTree(level + 1, sf::FloatRect(x, y, subWidth, subHeight));
	this->nodes[2] = new QuadTree(level + 1, sf::FloatRect(x, y + subHeight, subWidth, subHeight));
	this->nodes[3] = new QuadTree(level + 1, sf::FloatRect(x + subWidth, y + subHeight, subWidth, subHeight));
}

int Asteroid::QuadTree::getIndex(sf::FloatRect objPosition)
{
	int index = -1;
	float verticalMidpoint = this->bounds.left + this->bounds.width / 2.0f;
	float horizontalMidpoint = this->bounds.top + this->bounds.height / 2.0f;

	bool topQuadrant = (objPosition.top < horizontalMidpoint && objPosition.top + objPosition.height < horizontalMidpoint);
	bool bottomQuadrant = (objPosition.top > horizontalMidpoint);

	if (objPosition.left < verticalMidpoint && objPosition.left + objPosition.width < verticalMidpoint) {
		if (topQuadrant)
			index = 1;
		else if (bottomQuadrant)
			index = 2;
	}
	else if (objPosition.left > verticalMidpoint) {
		if (topQuadrant)
			index = 0;
		else if (bottomQuadrant)
			index = 3;
	}

	return index;
}

void Asteroid::QuadTree::insert(Meteor* meteor)
{
	if (this->nodes[0] != nullptr) {
		int index = getIndex(meteor->getSprite()->getGlobalBound());

		if (index != -1)
		{
			this->nodes[index]->insert(meteor);
			return;
		}
	}

	this->object.push_back(meteor);

	if (this->object.size() > MAX_OBJS && level < MAX_LEVELS) {
		if (this->nodes[0] == nullptr)
			this->split();


		for (unsigned int i = 0; i < this->object.size(); i++)
		{
			int index = this->getIndex(this->object[i]->getSprite()->getGlobalBound());
			if (index != -1)
			{
				Meteor* mt = this->object[i];
				this->object.erase(std::find(this->object.begin(), this->object.end(), mt));
				this->nodes[index]->insert(mt);
			}
		}
	}
}

std::vector<Asteroid::Meteor*>& Asteroid::QuadTree::retrieve(std::vector<Meteor*>& currentObject, sf::FloatRect bound)
{
	int index = this->getIndex(bound);
	if (index != -1 && this->nodes[0] != nullptr)
		this->nodes[index]->retrieve(currentObject, bound);

	for (unsigned int i = 0; i < this->object.size(); i++)
		currentObject.push_back(this->object[i]);

	return currentObject;
}

void Asteroid::QuadTree::removeMeteor(Meteor* meteor)
{
	bool found = false;

	std::vector<Meteor*>::iterator it = std::find(this->object.begin(), this->object.end(), meteor);
	found = it != this->object.end();

	if (found)
		this->object.erase(it);
	else
	{
		if (nodes[0] == nullptr)
			return;

		nodes[0]->removeMeteor(meteor);
		nodes[1]->removeMeteor(meteor);
		nodes[2]->removeMeteor(meteor);
		nodes[3]->removeMeteor(meteor);
	}
}