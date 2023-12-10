#pragma once
#include "Soldier.h"
class normalSoldier :public Soldier{

	// GUI vars

	const int radius = 40;

public:
	normalSoldier() = default;
	normalSoldier(const SoldierColor color, sf::Vector2i pos);
	void move(list<sf::Vector2i> pos, Board* others) override;
	vector<list<sf::Vector2i>> getMoveLogic(Board* others) override;

	// GUI methods

	void draw(sf::RenderWindow* rwin) override;
};

