#include "normalSoldier.h"
#include <functional>


vector<list<sf::Vector2i>> normalSoldier::getMoveLogic(Board* board) {
    vector<list<sf::Vector2i>> posMoves;

    // add the diaganal right move
    sf::Vector2i right = position + sf::Vector2i(1, getDirection()); 
    if (Board::inBound(right) && !board->isFull(right)) {
        list<sf::Vector2i> rightMove;
        rightMove.push_front(right);
        posMoves.push_back(rightMove);
    }

    sf::Vector2i left = position + sf::Vector2i(-1, getDirection());
    if (Board::inBound(left) && !board->isFull(left)) {
        list<sf::Vector2i> leftMove;
        leftMove.push_front(left);
        posMoves.push_back(leftMove);
    }
    

    std::function<void(list<sf::Vector2i>, SoldierColor, std::vector<sf::Vector2i>)> getPossebleEatingMoves;
    getPossebleEatingMoves = [&](list<sf::Vector2i> path, SoldierColor color, std::vector<sf::Vector2i> killBag) -> void {
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                sf::Vector2i dir(i, j); 
                sf::Vector2i prayPos = (path.empty()?position: path.back()) + dir;
                sf::Vector2i nextPos = (path.empty() ? position : path.back()) + dir * 2;
                Soldier* foundPray = board->getSoldier(prayPos);  
                Soldier* foundNext = board->getSoldier(nextPos);
                if (Board::inBound(nextPos) && // make sure we dont go overboard
                    (!foundNext || !foundNext->isAlive()) && //make sure that there isn't someone in the nextPos 
                    foundPray && //make sure that there is someone in the prayPos 
                    foundPray->isAlive() && //make sure that the pray is alive 
                    foundPray->getColor()^color && //make sure that the someone is an enemy 
                    std::find(killBag.begin(), killBag.end(), prayPos) == killBag.end()) { //make sure that i dident kill this guy all ready
                    path.push_back(nextPos);
                    posMoves.push_back(path);
                    killBag.push_back(prayPos); 
                    getPossebleEatingMoves(path, color, killBag);
                }
            }
        }
    };
    getPossebleEatingMoves(list<sf::Vector2i>(), this->getColor(), std::vector<sf::Vector2i>());

    return posMoves;
}

Soldier* normalSoldier::clone() const
{
    return new normalSoldier(*this);
}

sf::Shape* normalSoldier::cloneShape()
{
    return build();
}

void normalSoldier::draw(sf::RenderWindow* rwin)
{
    if (isAlive()) {
        rwin->draw(*build());
    }
}


sf::Shape* normalSoldier::build()
{
    shape = new sf::CircleShape; 
    sf::CircleShape* circleShape = static_cast<sf::CircleShape*>(shape); 
    circleShape->setFillColor(this->getColor() ? sf::Color::Magenta : sf::Color::Cyan); 
    circleShape->setOrigin(radius, radius); 
    circleShape->setPosition(Board::posFromBoardCord(position)); 
    circleShape->setRadius(radius); 
    circleShape->setOutlineColor(sf::Color::Black); 
    circleShape->setOutlineThickness(-5); 
    return circleShape;
}

normalSoldier::normalSoldier(const SoldierColor color, sf::Vector2i pos) : Soldier(color, pos) {
    this->type = NORMAL;
}

normalSoldier::normalSoldier(const normalSoldier& other){
    this->type = NORMAL;
    this->position = other.position;
    this->alive = other.alive;
    this->color = other.color;
    this->shape = nullptr;
}

void normalSoldier::move(list<sf::Vector2i> path, Board* others)
{
    for (auto it = path.begin(); it != path.end(); it++) {
        if (position != *it && (position.x - it->x) % 2 == 0 && (position.y - it->y) % 2 == 0) {
            others->kill((position + *it) / 2);
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