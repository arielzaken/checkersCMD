#pragma once
#include <iostream>
#include <vector>
#include <list>
#include "Vector2D.h"
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
	Vector2D position;
public:
	Soldier() = default;
	Soldier(const SoldierColor& color, Vector2D pos) : color(color), position(pos), alive(true) {}

	SoldierColor getColor();
	Vector2D getPosition() const;
	
	bool isAlive() { return alive; }
	void kill() { alive = false; }

	int getDirection();
	virtual bool move(list<Vector2D> pos, Board* others) = 0;
	virtual vector<list<Vector2D>> getMoveLogic(Board* others) = 0;
};

