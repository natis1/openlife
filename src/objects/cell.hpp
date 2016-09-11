#pragma once
#include "entity.hpp"
#include <vector>
#include <iostream>
#include "../tools/tools.hpp"

class Cell;

//Alias
//using CellVec = std::vector<std::shared_ptr<Cell>>;

class Cell : public Entity
{
    
    // Effects of shared_ptr are undermined by static variables
    static float mate_radius;
    static float reproduce_chance;
    static float neighbor_radius;
    static float move_amount;
    static int   max_neighbors;

public:
    Cell();
    Cell(Cell& a, Cell& b);

    virtual void update();
    void interact(const std::vector<std::shared_ptr<Cell>>& cells);

    std::vector<std::shared_ptr<Cell>> mate();

    void bounce(sf::Vector2<unsigned int> bounds);

private:
    
    
    
    // After this reaches a threshhold a stork comes down and provides the cells with babies
    double affection = 0;
    // The change in affection over time, represented by the cell's red. Change the divisor to effect change in reproduction rates.
    double affectionPrime = ( (double) getFillColor().r ) / 92.;
    
    
    // The change in angle over time in degrees. Negative is to the left and vicea versa
    double anglePrime = (( (double) getFillColor().b ) - 127.)  / 1500.;
    
    
    
    
    
    int neighbors;
    std::vector<std::shared_ptr<Cell>> mates;
};

