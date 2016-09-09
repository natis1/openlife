#pragma once
#include "entity.hpp"
#include "food.hpp"
#include "../tools/tools.hpp"

class Cell;

//Alias
using CellVec = std::vector<std::shared_ptr<Cell>>;

class Cell : public Entity
{
static float reproduce_chance;
static float neighbor_radius;

public:
    Cell();
    Cell(Cell& a, Cell& b);

    virtual void update(FoodVec& food);
    void updateNeighbors(const CellVec& cells);

    CellVec mate();

    void bounce();

private:

    int neighbors;

    CellVec mates;
};

