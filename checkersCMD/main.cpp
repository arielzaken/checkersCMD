#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>
#include "Board.h"
#include "KingSoldier.h"
#include "MinMax.h"

// text box definishen becous i am lazy
struct TextBox {
	sf::Vector2f size;
	sf::Vector2f pos;
	sf::RectangleShape backGroundShape;
	sf::Text text;
	bool isSelected;

	TextBox(sf::Vector2f size, sf::Vector2f pos, sf::Font* font) {
		this->size = size;
		this->pos = pos;

		backGroundShape.setPosition(pos);
		backGroundShape.setSize(size);
		backGroundShape.setFillColor(sf::Color(255, 255, 255, 100));

		text.setFont(*font);
		text.setFillColor(sf::Color::White);
		text.setPosition(pos + sf::Vector2f(5.f, 5.f)); // Offset to show text within the box
		isSelected = false;

		// Calculate the character size based on the TextBox dimensions
		unsigned int characterSize = static_cast<unsigned int>(size.y * 0.6f); // Adjust as needed
		text.setCharacterSize(characterSize);
	}

	void setText(const sf::String& str) {
		text.setString(str);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(backGroundShape);
		window.draw(text);
	}
};

int main() {
	// create the window
	RenderWindow window(VideoMode(1000, 800), "Chessboard", Style::Close);

	//Textures
	sf::Texture boardTexture;
	if (!boardTexture.loadFromFile("textures/board.png")){
		spdlog::error("cant load board.png");
		return EXIT_FAILURE;
	}

	sf::Texture crownTexture;
	if (!crownTexture.loadFromFile("textures/crown.png")){
		spdlog::error("cant load crown.png");
		return EXIT_FAILURE;
	}

	sf::Texture houerGlassTexture;
	if (!houerGlassTexture.loadFromFile("textures/hourglass.png")) {
		spdlog::error("cant load hourglass.png");
		return EXIT_FAILURE;
	}

	// fonts
	sf::Font game_overFont;
	if (!game_overFont.loadFromFile("fonts/game_over.ttf")) { // Replace "arial.ttf" with your font file path
		spdlog::error("cant load game_over.ttf");
		return EXIT_FAILURE;
	}

	sf::Font KarmaFutureFont;
	if (!KarmaFutureFont.loadFromFile("fonts/KarmaFuture.ttf")) { // Replace "arial.ttf" with your font file path
		spdlog::error("cant load KarmaFuture.ttf");
		return EXIT_FAILURE;
	}

	//houerGlass inisialsiaion
	struct HourGlass
	{
		sf::Sprite Spirit;
		sf::IntRect frameRect;
		const int totalFrames = 7; // Total frames in the sprite sheet
		const int switchFrame = 100;
		int currentFrame = 0;
		sf::Clock clock;
	}; 
	HourGlass hourGlass;
	hourGlass.Spirit.setTexture(houerGlassTexture);
	hourGlass.Spirit.setScale(sf::Vector2f(0.3, 0.3));
	hourGlass.Spirit.setPosition(sf::Vector2f(350, 350));
	hourGlass.frameRect = sf::IntRect(0, 150, 271, 400);
	
	
	TextBox turn(Vector2f(180,50),Vector2f(810,10),&KarmaFutureFont);
	turn.text.setCharacterSize(25);
	TextBox lastTurn(Vector2f(180, 100), Vector2f(810, 70), &KarmaFutureFont);
	lastTurn.text.setCharacterSize(16);

	// turn box

	// board
	Board board(&boardTexture);
	board.set();  
	board.setSoldierTexture(&crownTexture);

	// start minmax
	MinMax minmax;


	// local parameters
	sf::Vector2i clickPos;
	sf::Vector2i from;
	sf::Vector2i to;

	bool lock = false; // if the mouse have bean pressed befor? (drag)
	bool validFrom = false;
	bool validTo = false;
	Soldier* fromSold = nullptr;

	std::vector<std::list<Vector2i>> posibleMoves;
	std::list<Vector2i> path;


	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		Event event;
		while (window.pollEvent(event)){
			if(event.type == Event::Closed)
				window.close();

			// check if left butten is pressed -------------------------------------------------------
			if (Mouse::isButtonPressed(Mouse::Left)) {
				sf::Vector2i clickPos = Mouse::getPosition(window);
				if (!lock) { // first press
					lock = true;
					if (board.isInBoard(&window, clickPos)) { // board manage
						from = Board::boardCordFromPos(clickPos);
						to = from;
						fromSold = board.getSoldier(from);
						if (fromSold && !fromSold->getColor() && !board.getTurn() && fromSold->isAlive()) {
							board.print();
							posibleMoves = fromSold->getMoveLogic(&board);
						}
						else
							posibleMoves = std::vector<std::list<Vector2i>>();
					}
				}
				else { // drag
					if (board.isInBoard(&window, clickPos)) { // board manage
						to = Board::boardCordFromPos(clickPos);
					}
				}
			}
			else {
				if (lock) { // end of drag
					lock = false;
					if (fromSold && Board::isListPresent(posibleMoves, path)) {
						board.move(from, path);
						board.print();
						board.switchTurn();
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

		// run the minmax 
		if (board.getTurn()) {
			if (minmax.isValid()) {
				Result res = minmax.getMove();
				board.move(res.from, res.move);
				std::string lastMove = '(' + std::to_string(res.from.x) + ',' + std::to_string(res.from.y) + ')';
				for (auto j = res.move.begin() ; j != res.move.end(); j++) {
					lastMove += " -> (" + std::to_string((j)->x) + ',' + std::to_string((j)->y) + ')';
				}
				lastTurn.setText(lastMove);
				board.switchTurn();

				board.print();
			}
			else if (!minmax.isThinking()) 
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
			fromFrameShape.setSize(sf::Vector2f(tileSize + 5, tileSize + 5));
			fromFrameShape.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
			fromFrameShape.setPosition(Board::posFromBoardCord(from));
			fromFrameShape.setFillColor(sf::Color::Transparent);
			fromFrameShape.setOutlineColor(sf::Color::Blue);
			fromFrameShape.setOutlineThickness(-5);
			window.draw(fromFrameShape);
		}

		if (lock && from != to) {
			RectangleShape toFrameShape;
			toFrameShape.setSize(sf::Vector2f(tileSize + 5, tileSize + 5));
			toFrameShape.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
			toFrameShape.setPosition(Board::posFromBoardCord(to));
			toFrameShape.setFillColor(sf::Color::Transparent);
			toFrameShape.setOutlineColor(sf::Color::Red);
			toFrameShape.setOutlineThickness(-5);
			window.draw(toFrameShape);
		}

		if (minmax.isThinking() && hourGlass.clock.getElapsedTime().asMilliseconds() >= hourGlass.switchFrame) { 
			hourGlass.clock.restart();
			// Update the frame of the hourglass animation
			hourGlass.currentFrame = (hourGlass.currentFrame + 1) % hourGlass.totalFrames; 
			hourGlass.frameRect.left = hourGlass.currentFrame * hourGlass.frameRect.width; 
			hourGlass.Spirit.setTextureRect(hourGlass.frameRect); 
		}

		// Draw the hourglass only when minmax is thinking
		if (minmax.isThinking()) { 
			sf::RectangleShape blurr;
			blurr.setSize(sf::Vector2f(800, 800));
			blurr.setFillColor(sf::Color(128,128,128,100));
			blurr.setPosition(sf::Vector2f(0,0));
			window.draw(blurr);
			window.draw(hourGlass.Spirit); 
		}

		turn.setText(board.getTurn() ? "TURN: BLACK" : "TURN: WHITE");
		turn.draw(window);
		lastTurn.draw(window);
		window.display();
	}
	return 0;
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