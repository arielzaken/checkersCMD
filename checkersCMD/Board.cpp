#include "Board.h"
#include "normalSoldier.h"
#include "KingSoldier.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

using namespace sf;

Board::Board(sf::Texture* _texture) : texture(_texture)
{
	print();
	turn = false;
}

Board::Board(const Board& other)
{
	// Copy texture and sprite (assuming they are pointers)
	this->texture = other.texture; // Shallow copy, assuming it's okay
	turn = false;
	// Deep copy of soldiers vector
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++){
			if (other.soldiers[i][j])
				this->soldiers[i][j] = other.soldiers[i][j]->clone();
		}
	}
}

Board::~Board()
{
	// Clean up memory by deleting allocated soldiers
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (soldiers[i][j])
				delete soldiers[i][j];
		}
	}
}

void Board::set()
{
	this->~Board();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			sf::Vector2i pos(i, j);
			if ((i + j) % 2)
				setSoldier(pos, new normalSoldier(BLACK, pos));
			else
				setSoldier(pos, nullptr);
		}
		for (int j = 5; j < 8; j++) {
			sf::Vector2i pos(i, j);
			setSoldier(pos, nullptr);
		}
		for (int j = 5; j < 8; j++) {
			sf::Vector2i pos(i, j);
			if ((i + j) % 2)
				setSoldier(pos, new normalSoldier(WHITE, pos));
			else
				setSoldier(pos, nullptr);
		}
	}
	//soldiers[5][4] = new normalSoldier(WHITE, sf::Vector2i(5, 4));
	//soldiers[4][5] = new normalSoldier(BLACK, sf::Vector2i(4, 5));
	//soldiers[1][4] = new normalSoldier(BLACK, sf::Vector2i(1, 4));
	//soldiers[2][5] = new normalSoldier(BLACK, sf::Vector2i(2, 5));
	//soldiers[6][7] = new KingSoldier(WHITE, sf::Vector2i(6, 7));

}

void Board::setSoldierTexture(sf::Texture* sText)
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (soldiers[i][j])
				soldiers[i][j]->setTexture(sText);
		}
	}
}

Soldier* Board::getSoldier(sf::Vector2i pos){
	if (inBound(pos))
		return soldiers[pos.x][pos.y]; 
	return nullptr;
}

bool Board::setSoldier(sf::Vector2i pos, Soldier* soldier)
{
	if (inBound(pos)) {
		soldiers[pos.x][pos.y] = soldier;
		return true;
	}
	return false; 
}

bool Board::getTurn()
{
	return turn;
}

void Board::switchTurn()
{
	turn = 1 - turn;
}

void Board::move(sf::Vector2i from, list<sf::Vector2i> path)
{
	Soldier* sold = getSoldier(from);
	if (sold != nullptr) {
		sold->move(path,this);
		setSoldier(from, getSoldier(path.back()));
		if (sold->getType() == NORMAL && ((path.back().y == 0 && sold->getDirection() == -1) || (path.back().y == 7 && sold->getDirection() == 1))) {
			Soldier* newKing = new KingSoldier((normalSoldier*)sold);
			delete sold;
			setSoldier(path.back(), newKing);
		}
		else
			setSoldier(path.back(), sold);
	}
}

void Board::kill(sf::Vector2i pos)
{
	Soldier* sold = getSoldier(pos);
	if (sold != nullptr) {
		delete sold;
		setSoldier(pos, nullptr);
	}
}

bool Board::isFull(sf::Vector2i pos) {
	if(inBound(pos))
		return soldiers[pos.x][pos.y] && soldiers[pos.x][pos.y]->isAlive();
	return false;
}

bool Board::inBound(list<sf::Vector2i> path)
{
	for (auto i = path.begin(); i != path.end(); i++)
		if (!inBound(*i))
			return false;
	return true;
}

bool Board::inBound(sf::Vector2i pos)
{
	return 0 <= pos.x && pos.x < 8 && 0 <= pos.y && pos.y < 8; 
}

void Board::print() {
	char charBoard[8][8];

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			if (soldiers[i][j]) {
				if(soldiers[i][j]->getColor())//Black
					charBoard[i][j] = soldiers[i][j]->getType() ? 'W' : 'w';
				else
					charBoard[i][j] = soldiers[i][j]->getType() ? 'B' : 'b';
			}
			else
				charBoard[i][j] = ' ';
		}

	system("cls");
	cout << "   0 1 2 3 4 5 6 7" << endl;
	for (int i = 0; i < 8; i++) {
		cout << i << " |";
		for (int j = 0; j < 8; j++)
			cout << charBoard[j][i] << "|";
		cout << endl;
	}
}

sf::Vector2f Board::posFromBoardCord(sf::Vector2i bc){
	return sf::Vector2f(tileSize * (bc.x + 0.5), tileSize * (bc.y + 0.5));
}

sf::Vector2i Board::boardCordFromPos(sf::Vector2i pos){
	return sf::Vector2i(pos.x / tileSize, pos.y / tileSize);
}

bool Board::isInBoard(sf::RenderWindow* rwin, sf::Vector2i pos)
{
	return 
		(this->windowPos.x < pos.x && pos.x < this->windowPos.x + this->size.x) &&
		(this->windowPos.y < pos.y && pos.y < this->windowPos.y + this->size.y);
}

void Board::draw(sf::RenderWindow* rwin)
{
	sf::Sprite boardSprite;
	boardSprite.setTexture(*texture);
	boardSprite.setScale(sf::Vector2f(tileSize / 100, tileSize / 100));
	boardSprite.setPosition(windowPos);
	rwin->draw(boardSprite);
}

bool Board::isListPresent(const std::vector<std::list<Vector2i>>& vecOfLists, const std::list<Vector2i>& searchList)
{
	auto it = std::find_if(vecOfLists.begin(), vecOfLists.end(), [&](const std::list<Vector2i>& listItem) {
		return listsAreEqual(listItem, searchList);
		});

	return it != vecOfLists.end();
}

template<typename T>
bool Board::listsAreEqual(const std::list<T>& list1, const std::list<T>& list2) {
	if (list1.size() != list2.size()) {
		return false;
	}

	auto it1 = list1.begin();
	auto it2 = list2.begin();

	while (it1 != list1.end() && it2 != list2.end()) {
		if (*it1 != *it2) {
			return false;
		}
		++it1;
		++it2;
	}

	return true;
}

void Board::drawSoldiers(sf::RenderWindow* rwin)
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (soldiers[i][j])
				soldiers[i][j]->draw(rwin);
		}
	}
}


