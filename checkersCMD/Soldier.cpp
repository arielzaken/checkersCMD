#include "Soldier.h"

int Soldier::getDirection()
{
    return color?-1:1;
}

Soldier::Soldier(const SoldierColor& color, sf::Vector2i pos) : color(color), position(pos), alive(true), shape(nullptr)
{

}

Soldier::Soldier(const Soldier& other)
{
    this->color = other.color;
    this->alive = other.alive;
    this->position = other.position; // Assuming sf::Vector2i supports copying directly
    // You might need to perform a deep copy of the shape if necessary
    // For example, if it's a pointer to an sf::Shape object
    // this->shape = new sf::Shape(*other.shape);
}

Soldier::~Soldier()
{
    if (shape)
        delete shape;
}

Soldier& Soldier::operator=(const Soldier& other)
{
    if (this != &other) {
        this->color = other.color;
        this->alive = other.alive;
        this->position = other.position; // Assuming sf::Vector2i supports copying directly
        // Perform a deep copy of the shape if necessary (similar to the copy constructor)
        // this->shape = new sf::Shape(*other.shape);
    }
    return *this;
}

SoldierColor Soldier::getColor()
{
    return color;
}

sf::Vector2i Soldier::getPosition() const
{
    return position;
}
