#pragma once
#include <SFML/Graphics.hpp>
#include "Soldier.h"
#include <list>
#include <thread>
using namespace std;
using namespace sf;

#define MAX_RECURSION 10

class MinMax
{
	struct Result{
		int choiceValue;
		list<sf::Vector2i> move;
	}; 
	Result min(Board* board, int n);
	Result max(Board* board, int n);
	int valueBoard(Board* board);

	void calcMove(Board* board, list<sf::Vector2i>* path, bool* thinking ,bool* valid);

	thread thread;
	bool thinking = false;
	bool valid = false;
	list<sf::Vector2i> path;
public:
	void startCalcMove(Board* board);
	bool isThinking() { return thinking; }
	bool isValid() { return valid; }
	list<sf::Vector2i> getMove() { return valid ? list<sf::Vector2i>() : path; }
};

