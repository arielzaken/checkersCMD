#pragma once
#include "Soldier.h"
#include <vector>

#define tileSize 100 // px

using namespace std;

class Soldier;
class Board
{
	vector<Soldier*> soldiers;
	// GUI vars

	sf::Texture* texture;
	sf::Sprite boardSprite;

public:

	const sf::Vector2f size = sf::Vector2f(tileSize * 8, tileSize * 8);
	const sf::Vector2f windowPos = sf::Vector2f(0, 0); // the right top corner pos in the window
	Board() :Board(nullptr) {}
	Board(sf::Texture* texture);
	Board(const Board& other);
	~Board();

	void set();// set the board in start pos
	vector<Soldier*>* getSoldiers() { return &soldiers; }
	Soldier* getSoldier(sf::Vector2i pos);

	bool isFull(sf::Vector2i pos);

	vector<Soldier*>::iterator find(sf::Vector2i pos);
	vector<Soldier*>::iterator begin() { return soldiers.begin(); }
	vector<Soldier*>::iterator end() { return soldiers.end(); }

	void print();

	// GUI stuff

	static sf::Vector2f posFromBoardCord(sf::Vector2i); // get the mid pos of the tile
	static sf::Vector2i boardCordFromPos(sf::Vector2i); // get the tile that the pos is above
	
	bool isInBoard(sf::RenderWindow* rwin, sf::Vector2i);

	void draw(sf::RenderWindow* rwin);
	void drawSoldiers(sf::RenderWindow* rwin);
};

