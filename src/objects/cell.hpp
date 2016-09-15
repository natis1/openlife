#pragma once
#include "entity.hpp"
#include <vector>
#include <iostream>
#include "../tools/tools.hpp"

namespace objects
{

class Cell : public Entity
{
    // Static variables don't live in class instances and therefore do not affect shared pointers
    const static float mate_radius;
    const static float neighbor_radius;
    const static float move_amount;

    const static int max_neighbors;
    const static int overpopulation_limit;
    const static int underpopulation_limit; 

    const static double underpopulation_damage;
    const static double overpopulation_damage;
    const static double affection_threshold;

public:
    Cell();
    Cell(Cell& a, Cell& b);

    virtual void update();
    void interact(const std::vector<std::shared_ptr<Cell>>& cells);
    void addNeighbor(std::shared_ptr<Cell> neighbor);

    std::vector<std::shared_ptr<Cell>> mate();

    void bounce(sf::Vector2f bounds);

private:
    // After this reaches a threshhold a stork comes down and provides the cells with babies
    double affection = 0;
    // The change in affection over time, represented by the cell's red. Change the divisor to effect change in reproduction rates.
    double affectionPrime = ( (double) getFillColor().r ) / 92.;
    
    // The change in angle over time in degrees. Negative is to the left and vicea versa
    double anglePrime = (( (double) getFillColor().b ) - 127.)  / 1500.;
    
    std::vector<std::shared_ptr<Cell>> mates;
    std::vector<std::shared_ptr<Cell>> neighbors;
};

}
