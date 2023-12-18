#pragma once
#include <vector>
#include "Soldier.h"

using namespace std;
using namespace sf;

#define tileSize 100 // px

class Soldier;

class Board
{
	Soldier* soldiers[8][8];
	bool turn;
	
	// GUI vars

	sf::Texture* texture;


public:

	const sf::Vector2f size = sf::Vector2f(tileSize * 8, tileSize * 8);
	const sf::Vector2f windowPos = sf::Vector2f(0, 0); // the right top corner pos in the window
	Board() :Board(nullptr) {}
	Board(sf::Texture* texture);
	Board(const Board& other);
	~Board();

	void set();// set the board in start pos
	void setSoldierTexture(sf::Texture* sText);
	Soldier* getSoldier(sf::Vector2i pos);
	bool setSoldier(sf::Vector2i pos, Soldier* soldier);

	bool getTurn();
	void switchTurn();

	void move(sf::Vector2i from, list<sf::Vector2i> path);
	void kill(sf::Vector2i pos);



	template<typename T>
	static bool listsAreEqual(const std::list<T>& list1, const std::list<T>& list2);
	static bool isListPresent(const std::vector<std::list<Vector2i>>& vecOfLists, const std::list<Vector2i>& searchList);

	bool isFull(sf::Vector2i pos);
	static bool inBound(list<sf::Vector2i> path);
	static bool inBound(sf::Vector2i pos);

	void print();

	// GUI stuff

	static sf::Vector2f posFromBoardCord(sf::Vector2i); // get the mid pos of the tile
	static sf::Vector2i boardCordFromPos(sf::Vector2i); // get the tile that the pos is above
	
	bool isInBoard(sf::RenderWindow* rwin, sf::Vector2i);

	void draw(sf::RenderWindow* rwin);
	void drawSoldiers(sf::RenderWindow* rwin);
};

