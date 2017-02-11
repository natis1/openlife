#pragma once
#include <vector>
#include <iostream>

#include "tools.hpp"

#include "entity.hpp"
#include "paramdict.hpp"


class Cell;

position getAverageLocation(std::vector<std::shared_ptr<Cell>> cells);

using tools::print;

class Cell : public objects::Entity 
{

public:

    Cell(double size, double life);
    Cell(double size, double life, Cell& a, Cell& b);
    
    float overpopulation_damage_taken  = 0.f;
    float underpopulation_damage_taken = 0.f;

    virtual void update(ParamDict& simulation_params);
    void interact(const std::vector<std::shared_ptr<Cell>>& cells, ParamDict& simulation_params);
    void addNeighbor(std::shared_ptr<Cell> neighbor);
    void addVisible (std::shared_ptr<Cell> cell);

    std::string csv();
    std::vector<std::shared_ptr<Cell>> mate(ParamDict& simulation_params);

    void bounce(double wallx, double wally, double wallwidth, double wallheight, double move_amount);

private:
    // After this reaches a threshhold a stork comes down and provides the cells with babies
    double affection = 0;
    
    std::vector<std::shared_ptr<Cell>> mates;
    std::vector<std::shared_ptr<Cell>> neighbors;
    std::vector<std::shared_ptr<Cell>> visible;

    void intelligentRotate(bool underpopulated, bool crowded, ParamDict& simulation_params);
};
