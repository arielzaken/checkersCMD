#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <list>
#include "Board.h"
using namespace std;

enum SoldierColor
{
	WHITE,
	BLACK
};

class Board;

class Soldier
{
	SoldierColor color;
	bool alive = true;

protected:

	sf::Vector2i position;

	// GUI vars

	sf::Shape* shape;

public:

	Soldier() = default;
	Soldier(const SoldierColor& color, sf::Vector2i pos);
	Soldier(const Soldier& other);
	~Soldier();

	Soldier& operator=(const Soldier& other);

	SoldierColor getColor();
	sf::Vector2i getPosition() const;
	
	bool isAlive() { return alive; }
	void kill() { alive = false; position = sf::Vector2i(-1,-1); }

	int getDirection();

	virtual void move(list<sf::Vector2i> pos, Board* others) = 0;
	virtual vector<list<sf::Vector2i>> getMoveLogic(Board* others) = 0;

	// GUI methods

	virtual void draw(sf::RenderWindow* rwin) = 0;
};

