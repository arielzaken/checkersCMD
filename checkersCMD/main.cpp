#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>
#include "Board.h"
#include "MinMax.h"

using namespace sf;

template<typename T>
bool listsAreEqual(const std::list<T>& list1, const std::list<T>& list2);
bool isListPresent(const std::vector<std::list<Vector2i>>& vecOfLists, const std::list<Vector2i>& searchList);

int main() {
	// create the window
	RenderWindow window(VideoMode(800, 1000), "Chessboard", Style::Close);

	//background
	sf::Texture boardTexture;
	if (!boardTexture.loadFromFile("board.png"))
		spdlog::error("cant load board");

	Board board(&boardTexture);
	board.set();

	MinMax minmax;

	sf::Vector2i from; 
	sf::Vector2i to;

	bool lock = false; // if the mouse have bean pressed befor? (drag)
	bool validFrom = false;
	bool validTo = false;

	SoldierColor turn = WHITE;
	std::vector<std::list<Vector2i>> posibleMoves;
	std::list<Vector2i> path;
	Soldier* fromSold = nullptr;
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		Event event;
		while (window.pollEvent(event)){
			if(event.type == Event::Closed)
				window.close();

			// check if left butten is pressed -------------------------------------------------------
			if (Mouse::isButtonPressed(Mouse::Left)){
				sf::Vector2i clickPos = Mouse::getPosition(window);
				if (!lock) { // first press
					lock = true;
					if (board.isInBoard(&window, clickPos)) { // board manage
						from = board.boardCordFromPos(clickPos);
						to = from;
						fromSold = board.getSoldier(from);
						if (fromSold && !fromSold->getColor() && fromSold->isAlive()) {
							board.print();
							posibleMoves = fromSold->getMoveLogic(&board);
							for (auto i = posibleMoves.begin(); i != posibleMoves.end(); i++) {
								auto j = i->begin();
								cout << '(' << (j++)->x << ',' << (j)->y << ')';
								for (; j != i->end(); j++) {
									if (i->size() > 1) {
										cout << " -> ";
										cout << '(' << (j)->x << ',' << (j)->y << ')';
									}
								}
								cout << endl;

							}
						}
						else
							posibleMoves = std::vector<std::list<Vector2i>>();
					}
				}
				else { // drag
					if (board.isInBoard(&window, clickPos)) { // board manage
						to = board.boardCordFromPos(clickPos);
					}
				}
			}
			else { 
				if (lock) { // end of drag
					lock = false;
					if (fromSold && isListPresent(posibleMoves, path)) {
						fromSold->move(path, &board);
						board.print();
						turn = SoldierColor( 1 - turn);
						posibleMoves = std::vector<std::list<Vector2i>>();
					}
					path = list<Vector2i>();
				}
				else { // just not clicked...

				}
			}

			// check if right butten is pressed -------------------------------------------------------
			if (Mouse::isButtonPressed(Mouse::Right)) {
				path.push_back(to);
			}

			// check if esc butten is pressed -------------------------------------------------------
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				path = list<Vector2i>();
			}
		}

		if (turn) {
			if (minmax.isValid()) 
				// TODO: stuff///....lnksdfbjFDSAnl
			else
				minmax.startCalcMove(&board);
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		board.draw(&window);

		for (auto i = posibleMoves.begin(); i != posibleMoves.end(); i++)  
			for (auto j = i->begin(); j != i->end(); j++) {  
				RectangleShape validTileShape;  
				validTileShape.setSize(sf::Vector2f(tileSize, tileSize));  
				validTileShape.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));  
				validTileShape.setPosition(Board::posFromBoardCord(*j)); 
				validTileShape.setFillColor(sf::Color::Green);  
				window.draw(validTileShape); 
			}

		for (auto i = path.begin(); i != path.end(); i++) {
			RectangleShape pathTileShape;
			pathTileShape.setSize(sf::Vector2f(tileSize, tileSize));
			pathTileShape.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
			pathTileShape.setPosition(Board::posFromBoardCord(*i));
			pathTileShape.setFillColor(sf::Color(255, 255, 0));
			window.draw(pathTileShape);
		}

		board.drawSoldiers(&window);

		if (lock) {
			RectangleShape fromFrameShape;
			fromFrameShape.setSize(sf::Vector2f(tileSize + 5, tileSize +5));
			fromFrameShape.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
			fromFrameShape.setPosition(Board::posFromBoardCord(from));
			fromFrameShape.setFillColor(sf::Color::Transparent);
			fromFrameShape.setOutlineColor(sf::Color::Blue);
			fromFrameShape.setOutlineThickness(-5);
			window.draw(fromFrameShape);
		}

		if(lock && from != to){
			RectangleShape toFrameShape; 
			toFrameShape.setSize(sf::Vector2f(tileSize + 5, tileSize + 5));
			toFrameShape.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
			toFrameShape.setPosition(Board::posFromBoardCord(to));
			toFrameShape.setFillColor(sf::Color::Transparent);
			toFrameShape.setOutlineColor(sf::Color::Red);
			toFrameShape.setOutlineThickness(-5);
			window.draw(toFrameShape);
		}
		
		window.display();
	}
	return 0;
}


// Custom function to compare two lists for equality
template<typename T>
bool listsAreEqual(const std::list<T>& list1, const std::list<T>& list2) {
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

bool isListPresent(const std::vector<std::list<Vector2i>>& vecOfLists, const std::list<Vector2i>& searchList) {
	auto it = std::find_if(vecOfLists.begin(), vecOfLists.end(), [&](const std::list<Vector2i>& listItem) {
		return listsAreEqual(listItem, searchList);
		});

	return it != vecOfLists.end();
}

//#include <iostream>
//#include "Board.h"
//using namespace std;
//
//int main() {
//	Board board;
//	bool turn = 0;
//	sf::Vector2i from;
//	sf::Vector2i to;
//	while (true)
//	{
//		int temp;
//		vector<Soldier*>::iterator selection; 
//		do {
//			cout << "move " << (turn ? "B" : "W") << " from col" << endl;
//			cin >> temp;  
//			from.x = temp;  
//			cout << "move " << (turn ? "B" : "W") << " from row" << endl;
//			cin >> temp;  
//			from.y = temp;
//			selection = board.find(from);
//			if (selection == board.end() || (*selection)->getColor() ^ !turn) {
//				board.print();  
//				cout << "invalid from pos" << endl; 
//			}
//		} while (selection == board.end() || (*selection)->getColor() ^ !turn);
//		board.print();
//		cout << "possible moves for ";
//		cout << '(' << from.x << ',' << from.y << ')';
//		cout << endl;
//		auto posMovs = (*selection)->getMoveLogic(&board);
//		for (auto i = posMovs.begin(); i != posMovs.end(); i++) {
//			auto j = i->begin();
//			cout << '(' << (j++)->x << ',' << (j)->y << ')';
//			for (; j != i->end(); j++) {
//				if (i->size() > 1) {
//					cout << " -> ";
//					cout << '(' << (j)->x << ',' << (j)->y << ')';
//				}
//			}
//			cout << endl; 
//		}
//		if (posMovs.size() > 0) {
//			cout << "what its gona be?" << endl; 
//			cin >> temp;
//			(*selection)->move(posMovs[temp], &board);
//			board.print();
//			turn = 1 - turn;
//		}
//		else
//			cout << "nothing to do..." << endl;
//	}
//	return 0;
//}