#include "MinMax.h"

Result MinMax::min(Board* board, int n)
{
	Result worstResult;
	worstResult.choiceValue = INT32_MAX;
	worstResult.move = list<sf::Vector2i>();

	if (!n) {
		worstResult.choiceValue = valueBoard(board); 
		return worstResult;
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Soldier* sold = board->getSoldier(sf::Vector2i(i,j));
			if (sold && sold->isAlive()) {
				auto possMoves = sold->getMoveLogic(board);
				for (auto move = possMoves.begin(); move != possMoves.end(); move++) {
					if (Board::inBound(*move)) {
						Board* nextBoard = new Board(*board);
						nextBoard->move(sold->getPosition(), *move);
						Result result = max(nextBoard, n - 1);
						delete nextBoard;
						if (result.choiceValue < worstResult.choiceValue) {
							worstResult.choiceValue = result.choiceValue;
							worstResult.move = *move;
							worstResult.from = sold->getPosition();
						}
					}
				}
			}
		}
	}
	return worstResult;
}

Result MinMax::max(Board* board, int n)
{
	Result bestResult;
	bestResult.choiceValue = INT32_MIN;
	bestResult.move = list<sf::Vector2i>();

	if (!n) {
		bestResult.choiceValue = valueBoard(board);
		return bestResult;
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Soldier* sold = board->getSoldier(sf::Vector2i(i, j));
			if (sold && sold->getColor() && sold->isAlive()) {
				auto possMoves = sold->getMoveLogic(board);
				for (auto move = possMoves.begin(); move != possMoves.end(); move++) {
					if (Board::inBound(*move)) {
						Board* nextBoard = new Board(*board);
						nextBoard->move(sold->getPosition(), *move);
						Result result = min(nextBoard, n - 1); 
						delete nextBoard;
						if (result.choiceValue > bestResult.choiceValue) {
							bestResult.choiceValue = result.choiceValue;
							bestResult.move = *move;
							bestResult.from = sold->getPosition();
						}
					}
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
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Soldier* sold = board->getSoldier(sf::Vector2i(i, j));
			if (sold && sold->isAlive()) {
				value += sold->getColor() ? 1 : -1;
				if (sold->getColor())
					loss = false;
				else
					win = false;
			}
		}
	}
	if (win)
		return INT32_MAX;
	if (loss)
		return INT32_MIN;
	return value;
}

void MinMax::calcMove(Board* board, Result* path , bool* thinking, bool* valid)
{
	*thinking = true;
	*path = this->max(board, MAX_RECURSION);
	*thinking = false;
	*valid = true;
}

void MinMax::startCalcMove(Board* board) {
	valid = false;
	if (thread) {
		thread->join();
		delete thread;
	}
	thread = new std::thread(&MinMax::calcMove, this, board, &(this->path), &thinking, &valid);
}

