#pragma once
#include "entity.hpp"
#include "../tools/tools.hpp"

class Cell;

using CellVec = std::vector<std::shared_ptr<Cell>>;

class Cell : public Entity
{
static float reproduce_chance;
static float neighbor_radius;

public:
    Cell();
    Cell(Cell& a, Cell& b);

    virtual void update();
    void updateNeighbors(const CellVec& cells);

    CellVec mate();

    void bounce();

private:

    int neighbors;

    CellVec mates;
};

