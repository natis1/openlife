#pragma once
#include "entity.hpp"
#include "../tools/tools.hpp"

class Cell;

//Alias
using CellVec = std::vector<std::shared_ptr<Cell>>;

class Cell : public Entity
{
static float reproduce_chance;
static float neighbor_radius;
static float move_amount;
static int   max_neighbors;

public:
    Cell();
    Cell(Cell& a, Cell& b);

    virtual void update();
    void interact(const CellVec& cells);

    CellVec mate();

    void bounce();

private:

    int neighbors;

    CellVec mates;
};

