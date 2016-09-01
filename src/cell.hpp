#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "physics.hpp"

class Cell;

using CellVec = std::vector<std::shared_ptr<Cell>>;

class Cell : public sf::CircleShape
{
static float reproduce_chance;
static float neighbor_radius;

public:
    Cell();
    Cell(Cell& a, Cell& b);
    void updateNeighbors(const CellVec& cells);
    void update();
    CellVec mate();

    void bounce();
    bool alive();
    int neighbors;


    CellVec mates;

private:
    int life;
    int total_life;
};

