#pragma once
#include <SFML/Graphics.hpp>
#include "Soldier.h"
#include <list>
#include <thread>
using namespace std;
using namespace sf;

#define MAX_RECURSION 5
struct Result {
	int choiceValue = 0;
	list<sf::Vector2i> move;
	sf::Vector2i from;
};
class MinMax
{
	Result min(Board* board, int n);
	Result max(Board* board, int n);
	int valueBoard(Board* board);

	void calcMove(Board* board, Result* path, bool* thinking ,bool* valid);

	thread* thread;
	bool thinking = false;
	bool valid = false;
	Result path;
public:
	void startCalcMove(Board* board);
	bool isThinking() { return thinking; }
	bool isValid() { return valid; }
	Result getMove() { 
		if (valid) {
			valid = false;
			return path;
		}
		return Result(); 
	}
};

