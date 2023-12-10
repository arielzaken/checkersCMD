#include "MinMax.h"

MinMax::Result MinMax::min(Board* board, int n)
{
	Result worstResult;
	worstResult.choiceValue = INT32_MAX;
	worstResult.move = list<sf::Vector2i>();

	if (!n) {
		worstResult.choiceValue = valueBoard(board); 
		return worstResult;
	}
	for (auto sold = board->begin(); sold != board->end(); sold++) {
		if ((*sold)->isAlive()) {
			auto possMoves = (*sold)->getMoveLogic(board);
			for (auto move = possMoves.begin(); move != possMoves.end(); move++) {
				Board* nextBoard = new Board(*board);
				Soldier* newSold = nextBoard->getSoldier((*sold)->getPosition());
				newSold->move(*move, nextBoard);
				Result result = max(nextBoard, n - 1);
				delete nextBoard;
				if (result.choiceValue < worstResult.choiceValue) {
					worstResult.choiceValue = result.choiceValue;
					worstResult.move = *move;
				}
			}
		}
	}
	return worstResult;
}

MinMax::Result MinMax::max(Board* board, int n)
{
	Result bestResult;
	bestResult.choiceValue = INT32_MIN;
	bestResult.move = list<sf::Vector2i>();

	if (!n) {
		bestResult.choiceValue = valueBoard(board);
		return bestResult;
	}
	for (auto sold = board->begin(); sold != board->end(); sold++){
		if ((*sold)->isAlive()) {
			auto possMoves = (*sold)->getMoveLogic(board);
			for (auto move = possMoves.begin(); move != possMoves.end(); move++) { 
				Board* nextBoard = new Board(*board); 
				Soldier* newSold = nextBoard->getSoldier((*sold)->getPosition());
				newSold->move(*move, nextBoard);
				Result result = min(nextBoard, n-1);
				delete nextBoard;
				if (result.choiceValue > bestResult.choiceValue) {
					bestResult.choiceValue = result.choiceValue;
					bestResult.move = *move;
				}
			}
		}
	}
	return bestResult;
}

int MinMax::valueBoard(Board* board)
{
	int value = 0;
	bool win = true;
	bool loss = true;
	for (auto i = board->begin(); i != board->end(); i++) {
		if ((*i)->isAlive()) {
			value += (*i)->getColor() ? 1 : -1;
			if ((*i)->getColor())
				loss = false;
			else
				win = false;
		}
	}
	if (win)
		return INT32_MAX;
	if (loss)
		return INT32_MIN;
	return value;
}

void MinMax::calcMove(Board* board, list<sf::Vector2i>* path , bool* thinking, bool* valid)
{
	*thinking = true;
	*path = max(board, MAX_RECURSION).move;
	*thinking = false;
	*valid = true;
}

void MinMax::startCalcMove(Board* board) {
	valid = false;
	thread = std::thread(&MinMax::calcMove, this, board, &path, &thinking, &valid);
}

