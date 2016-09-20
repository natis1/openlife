#include "cell.hpp"

namespace objects
{

const float Cell::mate_radius      = 1;
const float Cell::neighbor_radius  = 5;
const float Cell::move_amount      = 0.1;

const int Cell::underpopulation_limit = 2;
const int Cell::overpopulation_limit  = 5;
const int Cell::max_neighbors = 5;

const double Cell::underpopulation_damage = 0.001;
const double Cell::overpopulation_damage  = 0.2;
const double Cell::affection_threshold = 1000.;

// Create initial cell
Cell::Cell() : 
    Entity(10, 100)
{
    auto bounds = getLocalBounds();
    setOrigin(bounds.width / 2, bounds.height / 2);
}

// Create child cell
Cell::Cell(Cell& a, Cell& b) :
    Cell::Cell()
{
    using tools::avg;

    setRotation(a.getRotation() + 90);

    auto colorA = a.getFillColor();
    auto colorB = b.getFillColor();

    // Child cells are averaged colors of parent cells
    setFillColor(sf::Color(avg(colorA.r, colorB.r),
                           avg(colorA.g, colorB.g),
                           avg(colorA.b, colorB.b)));

    auto posA = a.getPosition();
    auto posB = b.getPosition();

    // Spawn location for child cells
    auto dx = avg(posA.x, posB.x);
    auto dy = avg(posA.y, posB.y);

    setPosition(dx, dy);
}


void Cell::bounce(sf::Vector2f bounds)
{
    auto rotation = getRotation();
    auto radius   = getRadius();
    auto pos      = getPosition();

    float normal = 0; // Angle normal to surface

    if (pos.x - radius < 0)             // Left 
        normal = 0;
    else if (pos.x + radius > bounds.x) // Right
        normal = 180;
    else if (pos.y - radius < 0)        // Top
        normal = 90;
    else                                // Bottom
        normal = 270;

    float inverse_normal = (int)(normal + 180) % 360;

    setRotation((inverse_normal - rotation) + normal);
    moveVec(*this, Cell::move_amount);
}


// Build a list/count of neighbors and mates
void Cell::interact(const std::vector<std::shared_ptr<Cell>>& cells)
{
    for (auto& cell : cells)
    {
        auto dist   = distance(*this, *cell);
        auto radius = getRadius();

        // Mate with closer cells, treat further ones as neighbors, and ignore the rest
        if (dist < radius * Cell::mate_radius) 
        {
            mates.push_back(cell);
        }
        
        if (dist < radius * Cell::neighbor_radius)
        {
            addNeighbor(cell);
            cell->addNeighbor(std::make_shared<Cell>(*this));
        }
    }
}

void Cell::addNeighbor(std::shared_ptr<Cell> cell)
{
    neighbors.push_back(cell);
}

void Cell::update()
{
    setRotation(this->getRotation() + this->anglePrime);
    moveVec(*this, Cell::move_amount);
    
    if (neighbors.size() < Cell::underpopulation_limit)     // Underpopulation
    {
        damage(Cell::underpopulation_damage);
    }
    else if (neighbors.size() > Cell::overpopulation_limit) // Overpopulation
    {
        damage(Cell::overpopulation_damage);
    }

    const sf::Color *cellColor = &getFillColor();
    // Fade on death
    setFillColor(sf::Color(cellColor->r, cellColor->g, cellColor->b, 255 * lifePercent()));

    // Moved this to inside of update: this decreases the rate of mating (And makes it more realistic)
    affection += affectionPrime; 
}

std::vector<std::shared_ptr<Cell>> Cell::mate()
{
    std::vector<std::shared_ptr<Cell>> children;
    
    if (mates.size() > 0 and neighbors.size() < Cell::max_neighbors)
    {
        for (auto& mate : mates)
        {
            //The mating threshold stops 2 parents from falling too far in love.
            if (affection + mate->affection > Cell::affection_threshold)
            {
                affection = 0;
                mate->affection = 0;
                children.push_back(std::make_shared<Cell>(Cell(*this, *mate)));
            }
        }
    }

    // Interaction finished
    mates.clear();
    neighbors.clear();

    return children;
}

}
