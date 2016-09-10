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
    
    
    
    // Yep, it's exactly what it sounds like. After this reaches a threshhold the cells mate.
    double horniness = 0;
    // The change in horniness over time, determined by cell's red. Change the divisor to effect change in reproduction rates.
    double horninessPrime = ( (double) getFillColor().r ) / 92.;
    
    
    // The change in angle over time in degrees. Negative is to the left and vicea versa
    double anglePrime = (( (double) getFillColor().b ) - 127.)  / 1500.;
    
    
    
    
    
    int neighbors;
    std::vector<std::shared_ptr<Cell>> mates;
};

