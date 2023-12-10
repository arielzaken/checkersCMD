#include "Board.h"
#include "normalSoldier.h"
#include <algorithm>

Board::Board(sf::Texture* _texture) : texture(_texture)
{
	if (_texture) {
		boardSprite.setTexture(*_texture);
		boardSprite.setPosition(windowPos);
	}
	print();
}

Board::Board(const Board& other)
{
	// Copy texture and sprite (assuming they are pointers)
	this->texture = other.texture; // Shallow copy, assuming it's okay
	this->boardSprite = other.boardSprite; // Shallow copy, assuming it's okay

	// Deep copy of soldiers vector
	for (Soldier* soldier : other.soldiers) {
		// Assuming Soldier class has a copy constructor or can be copied using the appropriate method
		this->soldiers.push_back(new Soldier(*soldier)); // Creating a new copy of each Soldier
	}
}

Board::~Board()
{
	// Clean up memory by deleting allocated soldiers
	for (auto i = soldiers.begin(); i != soldiers.end(); i++)
		delete *i;
}

void Board::set()
{
	for (auto i = soldiers.begin(); i != soldiers.end(); i++)
		delete* i;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++)
			if ((i + j) % 2)
				soldiers.push_back(new normalSoldier(WHITE, sf::Vector2i(i, j)));
		for (int j = 5; j < 8; j++)
			if ((i + j) % 2)
				soldiers.push_back(new normalSoldier(BLACK, sf::Vector2i(i, j)));
	}
}

Soldier* Board::getSoldier(sf::Vector2i pos){
	auto temp = find(pos);
	return temp==soldiers.end()?nullptr: *find(pos);
}

bool Board::isFull(sf::Vector2i pos) {
	return std::any_of(soldiers.begin(), soldiers.end(),
		[&](Soldier* s) { return (s->getPosition() == pos) && s->isAlive(); });
}

vector<Soldier*>::iterator Board::find(sf::Vector2i pos) {
	return std::find_if(soldiers.begin(), soldiers.end(),
		[&](Soldier* s) { return (s->getPosition() == pos); });
}

void Board::print() {
	char charBoard[8][8];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			charBoard[i][j] = ' ';

	for (auto i = soldiers.begin(); i != soldiers.end(); i++) {
		if ((*i)->isAlive()) {
			sf::Vector2i pos = (*i)->getPosition();
			charBoard[pos.y][pos.x] = ((*i)->getColor() ? 'w' : 'b');
		}
		else{
			sf::Vector2i pos = (*i)->getPosition();
			charBoard[pos.y][pos.x] = ((*i)->getColor() ? 'm' : 'p');
		}
	}
	system("cls");
	cout << "   0 1 2 3 4 5 6 7" << endl;
	for (int i = 0; i < 8; i++) {
		cout << i << " |";
		for (int j = 0; j < 8; j++)
			cout << charBoard[i][j] << "|";
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
	rwin->draw(boardSprite);
}

void Board::drawSoldiers(sf::RenderWindow* rwin)
{
	for (auto i = soldiers.begin(); i != soldiers.end(); i++)
		(*i)->draw(rwin); 
}


