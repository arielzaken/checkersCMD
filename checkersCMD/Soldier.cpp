#include "Soldier.h"

int Soldier::getDirection()
{
    return color?1:-1;
}

Soldier::Soldier(const SoldierColor& color, sf::Vector2i pos) : color(color), position(pos), alive(true), shape(nullptr)
{

}


Soldier::~Soldier()
{
    if (shape != nullptr) {
        delete shape;
        shape = nullptr; // Set to nullptr after deletion
    }
}

SoldierColor Soldier::getColor()
{
    return color;
}

sf::Vector2i Soldier::getPosition() const
{
    return position;
}

SoldierType Soldier::getType()
{
    return type;
}

sf::Texture* Soldier::getTexture()
{
    return texture;
}
