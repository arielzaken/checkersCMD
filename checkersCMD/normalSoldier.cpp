#include "normalSoldier.h"
#include <functional>


vector<list<sf::Vector2i>> normalSoldier::getMoveLogic(Board* board) {
    vector<list<sf::Vector2i>> posMoves;

    // add the diaganal right move
    if (!board->isFull(position + sf::Vector2i(1, getDirection()))) {
        list<sf::Vector2i> rightMove;
        rightMove.push_front(position + sf::Vector2i(1, getDirection()));
        posMoves.push_back(rightMove);
    }


    if (!board->isFull(position + sf::Vector2i(-1, getDirection()))) {
        list<sf::Vector2i> leftMove;
        leftMove.push_front(position + sf::Vector2i(-1, getDirection()));
        posMoves.push_back(leftMove);
    }
    

    std::function<void(list<sf::Vector2i>, SoldierColor, std::vector<sf::Vector2i>)> getPossebleEatingMoves;
    getPossebleEatingMoves = [&](list<sf::Vector2i> path, SoldierColor color, std::vector<sf::Vector2i> killBag) -> void {
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                sf::Vector2i dir(i, j); 
                sf::Vector2i prayPos = path.back() + dir;
                sf::Vector2i nextPos = path.back() + dir * 2;
                std::vector<Soldier*>::iterator foundPray = board->find(prayPos);  
                std::vector<Soldier*>::iterator foundNext = board->find(nextPos);
                if ((foundNext == board->end() || !(*foundNext)->isAlive())&& //make sure that there isn't someone in the nextPos
                    foundPray != board->end() && //make sure that there is someone in the prayPos
                    (*foundPray)->isAlive() && //make sure that the pray is alive
                    (*foundPray)->getColor()^color && //make sure that the someone is an enemy
                    std::find(killBag.begin(), killBag.end(), prayPos) == killBag.end()) { //make sure that i dident kill this guy all ready
                    path.push_back(nextPos);
                    posMoves.push_back(path);
                    killBag.push_back(prayPos); 
                    getPossebleEatingMoves(path, color, killBag);
                }
            }
        }
    };

    list<sf::Vector2i> temp;
    temp.push_back(position);
    getPossebleEatingMoves(temp, this->getColor(), std::vector<sf::Vector2i>());

    return posMoves;
}

void normalSoldier::draw(sf::RenderWindow* rwin)
{
    if (isAlive()) {
        shape = new sf::CircleShape;
        sf::CircleShape* circleShape = static_cast<sf::CircleShape*>(shape);
        circleShape->setFillColor(this->getColor() ? sf::Color::Magenta : sf::Color::Cyan);
        circleShape->setOrigin(radius, radius);
        circleShape->setPosition(Board::posFromBoardCord(position));
        circleShape->setRadius(radius);
        circleShape->setOutlineColor(sf::Color::Black);
        circleShape->setOutlineThickness(-5);
        rwin->draw(*circleShape);
    }
}


normalSoldier::normalSoldier(const SoldierColor color, sf::Vector2i pos) : Soldier(color, pos) {}

void normalSoldier::move(list<sf::Vector2i> path, Board* others)
{
    for (auto it = path.begin(); it != path.end(); it++) {
        if (position != *it && (position.x - it->x) % 2 == 0 && (position.y - it->y) % 2 == 0) {
            auto deadMenIt = others->find((position + *it) / 2);
            if (deadMenIt != others->end())
                (*deadMenIt)->kill();
        }
        position = *it;
    }
}

/*
2
5
1
1
2
0
4
5
1
2
3
0

*/