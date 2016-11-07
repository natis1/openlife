#pragma once
#include <vector>
#include <iostream>

#include "../tools/tools.hpp"

#include "entity.hpp"
#include "genome.hpp"


namespace objects
{

class Cell;

sf::Vector2f getAverageLocation(std::vector<std::shared_ptr<Cell>> cells);

class Cell : public Entity
{
    // Static variables don't live in class instances and therefore do not affect shared pointers
    const static float mate_radius;
    const static float neighbor_radius;
    const static float move_modifier; // How fast a cell moves relative to its size
    const static float standard_radius; // The size of an average cell
    const static float minimum_radius;

    const static int max_neighbors;
    const static int overpopulation_limit;
    const static int underpopulation_limit; 

    const static double underpopulation_damage;
    const static double overpopulation_damage;
    const static double affection_threshold;
    const static double turn_rate;
    const static double max_life;

public:
    Cell();
    Cell(Cell& a, Cell& b);

    virtual void update();
    void interact(const std::vector<std::shared_ptr<Cell>>& cells);
    void addNeighbor(std::shared_ptr<Cell> neighbor);

    std::string csv();
    std::vector<std::shared_ptr<Cell>> mate();

    void bounce(sf::Vector2f bounds);

    Genome genome;

private:

    // After this reaches a threshhold a stork comes down and provides the cells with babies
    double affection = 0;
    
    std::vector<std::shared_ptr<Cell>> mates;
    std::vector<std::shared_ptr<Cell>> neighbors;

    void displayAttributes(); // Convert genome values to visible aspects, like size or color.
    
    double calculateIdealAngle(sf::Vector2f neighborLoc, double currentAngle);
    double calculateNextAngle(double currentAngle, bool isOverpopulated);

};

}
