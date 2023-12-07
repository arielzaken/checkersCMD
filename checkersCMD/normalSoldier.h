#pragma once
#include "Soldier.h"
class normalSoldier :public Soldier{
public:
	normalSoldier() = default;
	normalSoldier(const SoldierColor color, Vector2D pos);
	bool move(list<Vector2D> pos, Board* others) override;
	vector<list<Vector2D>> getMoveLogic(Board* others) override;
};

