#pragma once
#include "normalSoldier.h"
#include <functional>
class KingSoldier : public normalSoldier
{
	vector<sf::Vector2i> shotBeam(sf::Vector2i dir, SoldierColor color, Board* board);
	sf::Vector2i getEnemyPosOnBeam(sf::Vector2i start, sf::Vector2i dir, SoldierColor color, Board* board);
	vector<sf::Vector2i> getEnemysPosOnBeam(sf::Vector2i start, sf::Vector2i dir, SoldierColor color, Board* board);
	vector<sf::Vector2i> getEnemysPosOnSegment(sf::Vector2i start, sf::Vector2i finish, sf::Vector2i dir, SoldierColor color, Board* board);
protected:
public:
	KingSoldier() = default;
	KingSoldier(const SoldierColor color, sf::Vector2i pos);
	KingSoldier(normalSoldier* ns);
	virtual void move(list<sf::Vector2i> pos, Board* others) override;
	virtual vector<list<sf::Vector2i>> getMoveLogic(Board* others) override;
	virtual Soldier* clone() const override;
	virtual void draw(sf::RenderWindow* rwin) override;
};

