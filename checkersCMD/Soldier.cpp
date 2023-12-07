#include "Soldier.h"

int Soldier::getDirection()
{
    return color?1:-1;
}

SoldierColor Soldier::getColor()
{
    return color;
}

Vector2D Soldier::getPosition() const
{
    return position;
}
