#include "KingSoldier.h"

KingSoldier::KingSoldier(const SoldierColor color, sf::Vector2i pos):normalSoldier(color, pos)
{
    type = KING;
}

KingSoldier::KingSoldier(normalSoldier* ns):normalSoldier(*ns)
{
    this->texture = ns->getTexture();
    type = KING;
}

void KingSoldier::move(list<sf::Vector2i> path, Board* others)
{
    for (auto it = path.begin(); it != path.end(); it++) {
        if (position != *it) {
            auto deadMenIt = getEnemysPosOnSegment(position, *it, sf::Vector2i(position.x > it->x ? -1 : 1, position.y > it->y ? -1 : 1), getColor(), others);
            for (auto deadMen = deadMenIt.begin() ; deadMen != deadMenIt.end() && deadMen->x ; deadMen++)
                others->kill(*deadMen);
        }
        position = *it;
    }
}

vector<list<sf::Vector2i>> KingSoldier::getMoveLogic(Board* board)
{
    //board->print();
    vector<list<sf::Vector2i>> posMoves;

    std::function<void(list<sf::Vector2i>, SoldierColor, std::vector<sf::Vector2i>)> getPossebleEatingMoves;
    getPossebleEatingMoves = [&](list<sf::Vector2i> path, SoldierColor color, std::vector<sf::Vector2i> killBag) -> void {
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                sf::Vector2i dir(i, j);
                sf::Vector2i prayPos = getEnemyPosOnBeam(path.back(), dir, getColor(), board);
                if (prayPos != sf::Vector2i(-1,-1) && find(killBag.begin(), killBag.end(), prayPos) == killBag.end())
                    for (sf::Vector2i i = prayPos + dir; Board::inBound(i) && !board->isFull(i); i += dir) {
                        path.push_back(i);
                        posMoves.push_back(path);
                        killBag.push_back(prayPos);
                        getPossebleEatingMoves(path, color, killBag);
                    }
            }
        }
    };


    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            sf::Vector2i dir(i, j);
            vector<sf::Vector2i> posMovesInDir = shotBeam(dir, getColor(), board);
            for (auto i = posMovesInDir.begin(); i < posMovesInDir.end(); i++){
                list<sf::Vector2i> temp; 
                temp.push_back(*i);
                posMoves.push_back(temp);
                sf::Vector2i prayPos = getEnemyPosOnBeam(position, dir, getColor(), board);
                if (prayPos != sf::Vector2i(-1, -1)) {
                    std::vector<sf::Vector2i> killBag;
                    killBag.push_back(prayPos);
                    getPossebleEatingMoves(temp, getColor(), killBag);
                }
            }
        }
    }
    return posMoves;
}

Soldier* KingSoldier::clone() const
{
    return new KingSoldier(*this);
}

void KingSoldier::draw(sf::RenderWindow* rwin)
{
    if (isAlive()) {
        rwin->draw(*build());
        if (texture) {
            sf::Sprite textureSprite;
            textureSprite.setOrigin(50, 50);
            textureSprite.setPosition(Board::posFromBoardCord(position));
            textureSprite.setTexture(*texture);
            rwin->draw(textureSprite);
        }
    }
}

vector<sf::Vector2i> KingSoldier::shotBeam(sf::Vector2i dir, SoldierColor color, Board* board)
{
    vector<sf::Vector2i> beam;
    for (sf::Vector2i i = position + dir; Board::inBound(i) ; i+=dir){
        Soldier* cell = board->getSoldier(i);
        if (!cell || !cell->isAlive())
            beam.push_back(i);
        else if (cell->getColor() ^ !color)
            return beam;
    }
    return beam;
}

sf::Vector2i KingSoldier::getEnemyPosOnBeam(sf::Vector2i start, sf::Vector2i dir, SoldierColor color, Board* board)
{
    for (sf::Vector2i i = start; Board::inBound(i); i += dir) {
        Soldier* cell = board->getSoldier(i);
        if (cell && cell->getColor() ^ color)
            return i;
    }
    return sf::Vector2i(-1, -1);
}

vector<sf::Vector2i> KingSoldier::getEnemysPosOnBeam(sf::Vector2i start, sf::Vector2i dir, SoldierColor color, Board* board)
{
    vector<sf::Vector2i> res;
    for (sf::Vector2i i = start; Board::inBound(i); i += dir) {
        Soldier* cell = board->getSoldier(i);
        if (cell && cell->getColor() ^ !color)
            res.push_back(i);
    }
    return res;
}

vector<sf::Vector2i> KingSoldier::getEnemysPosOnSegment(sf::Vector2i start, sf::Vector2i finish, sf::Vector2i dir, SoldierColor color, Board* board)
{
    vector<sf::Vector2i> res;
    for (sf::Vector2i i = start + dir; Board::inBound(i) && i!=finish; i += dir) {
        Soldier* cell = board->getSoldier(i);
        if (cell && cell->getColor() ^ color)
            res.push_back(i);
    } 
    return res;
}
