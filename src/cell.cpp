#include "cell.hpp"
#include <iostream>

Cell::Cell() : 
    sf::CircleShape(10),
    life(100)
{
    // Create and seed a random number generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    // Distributions for angles in degrees
    std::uniform_int_distribution<std::mt19937::result_type> angleDist  (0, 360); 

    int angle = angleDist(rng);
    setRotation(angle);
}

void Cell::bounce()
{
    rotate(180);
}

void Cell::update()
{
    moveVec(*this, 0.5);
}
