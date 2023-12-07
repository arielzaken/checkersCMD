#pragma once
#include "Soldier.h"
#include <vector>
using namespace std;
class Soldier;
class Board
{
	vector<Soldier*> soldiers;
public:
	Board();
	bool isFull(Vector2D pos);
	vector<Soldier*>::iterator find(Vector2D pos);

	vector<Soldier*>::iterator begin() { return soldiers.begin(); }
	vector<Soldier*>::iterator end() { return soldiers.end(); }

	vector<Soldier*>* getSoldiers() { return &soldiers; }
};

