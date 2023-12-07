#include "normalSoldier.h"
#include <functional>
#include <set>


vector<list<Vector2D>> normalSoldier::getMoveLogic(Board* board) {
    vector<list<Vector2D>> posMoves;

    // add the diaganal right move
    if (board->isFull(position + Vector2D(1, getDirection()))) {
        list<Vector2D> rightMove;
        rightMove.push_front(position + Vector2D(1, getDirection()));
        posMoves.push_back(rightMove);
    }


    if (board->isFull(position + Vector2D(-1, getDirection()))) {
        list<Vector2D> leftMove;
        leftMove.push_front(position + Vector2D(-1, getDirection()));
        posMoves.push_back(leftMove);
    }
    

    std::function<void(list<Vector2D>, std::set<Vector2D>)> getPossebleEatingMoves;
    getPossebleEatingMoves = [&](list<Vector2D> path, std::set<Vector2D> killBag) -> void {
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                Vector2D dir(i, j); 
                Vector2D prayPos = path.front() + dir;  
                if (board->isFull(prayPos) && killBag.find(prayPos) == killBag.end()) {  
                    Vector2D nextPos = path.front() + dir * 2;
                    path.push_back(nextPos);
                    posMoves.push_back(path);
                    killBag.insert(prayPos); 
                    getPossebleEatingMoves(path, killBag);
                }
            }
        }
    };

    list<Vector2D> temp;
    temp.push_back(position);
    getPossebleEatingMoves(temp, std::set<Vector2D>());

    return posMoves;
}


normalSoldier::normalSoldier(const SoldierColor color, Vector2D pos) : Soldier(color, pos) {}

bool normalSoldier::move(list<Vector2D> path, Board* others)
{
	auto moves = getMoveLogic(others);
	list<Vector2D> requastedMove;
	for (auto it = moves.begin(); it != moves.end(); it++)
		if (*it == path)
			requastedMove = *it;
	if (requastedMove.size()) {
		for (auto it = requastedMove.begin(); it != requastedMove.end(); it++) {
			if ((position.getX() - it->getX()) % 2 == 0 && (position.getY() - it->getY()) % 2 == 0) {
				auto deadMenIt = others->find(position + (position - *it) * 0.5);
				if (deadMenIt != others->end()) 
					(*deadMenIt)->kill(); 
			}
			position = *it;
		}
		return true;
	}
	return false;
}