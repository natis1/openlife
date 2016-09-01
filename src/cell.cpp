#include "cell.hpp"
#include <iostream>

float Cell::reproduce_chance = .01;
float Cell::neighbor_radius  = 2;

// Create initial cell
Cell::Cell() : 
    sf::CircleShape(10),
    neighbors(0),
    life(1000),
    total_life(life)
{
    auto bounds = getLocalBounds();
    setOrigin(bounds.width / 2, bounds.height / 2);
}

// Create child cell
Cell::Cell(Cell& a, Cell& b) :
    Cell::Cell()
{
    const auto avg = [](auto x, auto y){return (x + y) / 2;};

    setRotation(avg(a.getRotation(), b.getRotation()));

    auto colorA = a.getFillColor();
    auto colorB = b.getFillColor();

    setFillColor(sf::Color(avg(colorA.r, colorB.r),
                           avg(colorA.g, colorB.g),
                           avg(colorA.b, colorB.b)));

    auto posA = a.getPosition();
    auto posB = b.getPosition();

    setPosition(avg(posA.x, posB.x), avg(posA.y, posB.y));
}


void Cell::bounce()
{
    rotate(180);
}

bool Cell::alive()
{
    return life >= 0;
}

void Cell::updateNeighbors(const std::vector<std::shared_ptr<Cell>>& cells)
{
    for (auto& cell : cells)
    {
        auto d = distance(*this, *cell);
        auto r = getRadius();
        // *2 means if the circles collide on edges (If left off, would be collision of edge with a center)
        if (d < r * 2) 
        {
            mates.push_back(cell);
        }
        
        if (d < r * 2 * Cell::neighbor_radius)
        {
            neighbors++;
            cell->neighbors++;
        }
    }
}

void Cell::update()
{
    moveVec(*this, .1);
    
    if (neighbors < 2)
    {
        life--;
    }
    else if (neighbors > 3)
    {
        life--;
    }
    else
    {
        life++;
    }

    neighbors = 0;

    auto color = getFillColor();
    float lifeFraction = (float)life / (float)total_life;
    setFillColor(sf::Color(color.r, color.g, color.b, 255 * lifeFraction));
}

std::vector<std::shared_ptr<Cell>> Cell::mate()
{
    std::vector<std::shared_ptr<Cell>> children;

    /*
    for (auto& mate : mates)
    {
        children.push_back(std::make_shared<Cell>(Cell(*this, *mate)));
    }
    */
    mates.clear();


    return children;
}
