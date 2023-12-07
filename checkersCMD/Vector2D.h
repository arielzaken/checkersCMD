#ifndef VECTOR2D_H
#define VECTOR2D_H


#include <iostream>
#include <cmath>

class Vector2D {
private:
    int x;
    int y;

public:
    // Constructor
    Vector2D(int xVal = 0, int yVal = 0) : x(xVal), y(yVal) {}

    // Getter functions
    int getX() const { return x; }
    int getY() const { return y; }

    // Setter functions
    void setX(int xVal) { x = xVal; }
    void setY(int yVal) { y = yVal; }

    // Addition of two vectors
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    // Subtraction of two vectors
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    // Scalar multiplication
    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    // Equality operator (==)
    bool operator==(const Vector2D& other) const {
        return (x == other.x) && (y == other.y);
    }

    // Inequality operator (!=)
    bool operator!=(const Vector2D& other) const {
        return !(*this == other); // Utilizes the equality operator to implement !=
    }

    bool operator<(const Vector2D& other) const {
        return this->squarLength() < other.squarLength();
    }

    // Magnitude of the vector
    double magnitude() const {
        return std::sqrt(x * x + y * y);
    }
    // the number of moves to get to this pos from 0,0
    double squarLength() const {
        return abs(x) + abs(y);
    }

    // Dot product of two vectors
    int dotProduct(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }

    // Print the vector
    void print() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};


#endif // !VECTOR2D_H