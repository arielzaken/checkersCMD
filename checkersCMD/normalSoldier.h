#pragma once
#include "Soldier.h"

constexpr auto radius = 40;

class normalSoldier :public Soldier{
protected:
	// GUI vars

	virtual sf::Shape* build();
public:
	normalSoldier() = default;
	normalSoldier(const SoldierColor color, sf::Vector2i pos);
	normalSoldier(const normalSoldier& other);
	void move(list<sf::Vector2i> pos, Board* others) override;
	vector<list<sf::Vector2i>> getMoveLogic(Board* others) override;
	// Implementing the clone function for ConcreteSoldier
	virtual Soldier* clone() const override;
	virtual sf::Shape* cloneShape() override;


	// GUI methods
	
	void draw(sf::RenderWindow* rwin) override;
};

