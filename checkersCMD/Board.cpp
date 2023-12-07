#include "Board.h"
#include "normalSoldier.h"
#include <algorithm>

Board::Board()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)
			if ((i + j) % 2)
				soldiers.push_back(new normalSoldier(WHITE, Vector2D(i, j))); 
	for (int i = 0; i < 8; i++)
		for (int j = 5; j < 8; j++)
			if ((i + j) % 2)
				soldiers.push_back(new normalSoldier(BLACK, Vector2D(i, j))); 
}

bool Board::isFull(Vector2D pos) {
	return std::any_of(soldiers.begin(), soldiers.end(),
		[&](const Soldier* s) { return s->getPosition() == pos; });
}

vector<Soldier*>::iterator Board::find(Vector2D pos) {
	return std::find_if(soldiers.begin(), soldiers.end(),
		[&](const Soldier* s) { return s->getPosition() == pos; });
}

