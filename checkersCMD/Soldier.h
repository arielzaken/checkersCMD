#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <list>
#include "Board.h"
using namespace std;

static enum SoldierColor
{
	WHITE,
	BLACK
};

static enum SoldierType
{
	NORMAL,
	KING
};

class Board;

class Soldier
{

protected:

	SoldierColor color; 
	SoldierType type;
	bool alive = true;
	sf::Vector2i position;

	// GUI vars

	sf::Shape* shape = nullptr;
	sf::Texture* texture = nullptr;
public:

	Soldier() = default;
	Soldier(const SoldierColor& color, sf::Vector2i pos);
	~Soldier();

	SoldierColor getColor();
	sf::Vector2i getPosition() const;
	SoldierType getType();
	sf::Texture* getTexture();
	
	bool isAlive() { return alive; }
	void kill() { alive = false; position = sf::Vector2i(-1,-1); }

	int getDirection();

	virtual void move(list<sf::Vector2i> pos, Board* others) = 0;
	virtual vector<list<sf::Vector2i>> getMoveLogic(Board* others) = 0;
	virtual Soldier* clone() const = 0;
	virtual sf::Shape* cloneShape() = 0;


	// GUI methods
	void setTexture(sf::Texture* _texture) { texture = _texture; }
	virtual void draw(sf::RenderWindow* rwin) = 0;
};

