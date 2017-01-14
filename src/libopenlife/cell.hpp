#pragma once
#include <vector>
#include <iostream>

#include "tools.hpp"

#include "entity.hpp"
#include "genome.hpp"


class Cell;

position getAverageLocation(std::vector<std::shared_ptr<Cell>> cells);

using tools::print;

class Cell : public objects::Entity 
{
    // Static variables don't live in class instances and therefore do not affect shared pointers
    const static float mate_radius;
    const static float neighbor_radius;
    const static float search_radius;
    const static float move_modifier;

    const static int overpopulation_limit;
    const static int underpopulation_limit;
    const static int crowded_number;

    const static double regeneration_amount;
    const static double underpopulation_damage;
    const static double overpopulation_damage;
    const static double affection_threshold;
    const static double turn_rate;
    const static double max_life;

public:
    Cell();
    Cell(Cell& a, Cell& b);
    
    float overpopulation_damage_taken  = 0.f;
    float underpopulation_damage_taken = 0.f;

    virtual void update();
    void interact(const std::vector<std::shared_ptr<Cell>>& cells);
    void addNeighbor(std::shared_ptr<Cell> neighbor);
    void addVisible (std::shared_ptr<Cell> cell);

    std::string csv();
    std::vector<std::shared_ptr<Cell>> mate();

    void bounce(double wallx, double wally, double wallwidth, double wallheight);

    Genome genome;

private:
    // After this reaches a threshhold a stork comes down and provides the cells with babies
    double affection = 0;
    
    std::vector<std::shared_ptr<Cell>> mates;
    std::vector<std::shared_ptr<Cell>> neighbors;
    std::vector<std::shared_ptr<Cell>> visible;

    void intelligentRotate(bool overpopulated);
    
};
