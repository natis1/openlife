#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "physics.hpp"

class Cell : public sf::CircleShape
{
static float reproduce_chance;
static float neighbor_radius;

public:
    Cell();
    Cell(Cell& a, Cell& b);
    void updateNeighbors(const std::vector<std::shared_ptr<Cell>>& cells);
    void update();
    std::vector<std::shared_ptr<Cell>> mate();

    void bounce();
    bool alive();
    int neighbors;


    std::vector<std::shared_ptr<Cell>> mates;

private:
    int life;
    int total_life;
};

