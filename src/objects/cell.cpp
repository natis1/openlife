#include "cell.hpp"

float Cell::reproduce_chance = .01;
float Cell::neighbor_radius  = 2;

// Create initial cell
Cell::Cell() : 
    neighbors(0)
{
    auto bounds = getLocalBounds();
    setOrigin(bounds.width / 2, bounds.height / 2);
}

// Create child cell
Cell::Cell(Cell& a, Cell& b) :
    Cell::Cell()
{
    const auto avg = [](auto x, auto y){return (x + y) / 2;};

    setRotation(a.getRotation());

    auto colorA = a.getFillColor();
    auto colorB = b.getFillColor();

    setFillColor(sf::Color(avg(colorA.r, colorB.r),
                           avg(colorA.g, colorB.g),
                           avg(colorA.b, colorB.b)));

    auto posA = a.getPosition();
    auto posB = b.getPosition();

    auto dx = avg(posA.x, posB.x);
    auto dy = avg(posA.y, posB.y);

    auto radii = avg(a.getRadius(), b.getRadius());

    auto toleranceDist = dist(0, radii);

    auto generator = randomGenerator();

    dx += ((radii + toleranceDist(generator))* sign(generator));
    dy += ((radii + toleranceDist(generator))* sign(generator));

    setPosition(dx, dy);
}


void Cell::bounce()
{
    rotate(180);
}


void Cell::updateNeighbors(const std::vector<std::shared_ptr<Cell>>& cells)
{
    for (auto& cell : cells)
    {
        auto d = distance(*this, *cell);
        auto r = getRadius();
        if (d < r) 
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
    /*
    auto min = std::min_element(food.begin(), food.end(),
        [this]( const auto &a, const auto &b )
        {
        return distance(*a, *this) < distance(*b, *this);
        } );

    auto angle_to_food = angle(**min, *this);
    setRotation(angle_to_food);
    */

    moveVec(*this, .1);
    
    if (neighbors < 2)
    {
        damage(1);
    }
    else if (neighbors > 3)
    {
        damage(1);
    }
    else
    {
        regen(1);
    }

    neighbors = 0;

    auto color = getFillColor();
    setFillColor(sf::Color(color.r, color.g, color.b, 255 * lifePercent()));
}

CellVec Cell::mate()
{
    CellVec children;

    auto generator = randomGenerator();
    auto chanceDist = dist(0, 1000000);

    if (mates.size() > 0)
    {
        for (auto& mate : mates)
        {
            if (chanceDist(generator) < 10)
            {
                children.push_back(std::make_shared<Cell>(Cell(*this, *mate)));
            }
        }
    }
    mates.clear();


    return children;
}
