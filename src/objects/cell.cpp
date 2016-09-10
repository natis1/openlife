#include "cell.hpp"

float Cell::mate_radius      = 1;
float Cell::reproduce_chance = 1;
float Cell::neighbor_radius  = 5;
float Cell::move_amount      = 0.1;

int Cell::max_neighbors = 10;

// Create initial cell
Cell::Cell() : 
    neighbors(0),
    Entity(10, 10000)
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

    // Child cells are averaged colors of parent cells
    setFillColor(sf::Color(avg(colorA.r, colorB.r),
                           avg(colorA.g, colorB.g),
                           avg(colorA.b, colorB.b)));

    auto posA = a.getPosition();
    auto posB = b.getPosition();

    // Spawn location for child cells
    auto dx = avg(posA.x, posB.x);
    auto dy = avg(posA.y, posB.y);

    auto radii = avg(a.getRadius(), b.getRadius());

    // A tolerance around spawn distance prevents children from mating with their parents..
    auto toleranceDist = dist(0, (int)radii);

    auto generator = randomGenerator();

    dx += ((radii + toleranceDist(generator))* sign(generator));
    dy += ((radii + toleranceDist(generator))* sign(generator));

    setPosition(dx, dy);
}


void Cell::bounce(sf::Vector2<unsigned int> bounds)
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
        auto dist = distance(*this, *cell);
        
        auto radius          = getRadius();

        // Mate with closer cells, treat further ones as neighbors, and ignore the rest
        if (dist < radius * Cell::mate_radius) 
        {
            mates.push_back(cell);
        }
        
        if (dist < radius * Cell::neighbor_radius)
        {
            neighbors++;
            cell->neighbors++;
        }
    }
}

void Cell::update()
{
    moveVec(*this, Cell::move_amount);
    
    if (neighbors < 2)
    {
        damage(1);
    }
    else if (neighbors > 3)
    {
        damage(10);
    }
    else
    {
        //regen(1);
    }


    auto color = getFillColor();
    setFillColor(sf::Color(color.r, color.g, color.b, 255 * lifePercent()));
}

CellVec Cell::mate()
{
    CellVec children;

    auto chanceDist = dist(0.0, 100.0);
    auto generator  = randomGenerator();

    if (mates.size() > 0 and neighbors < Cell::max_neighbors)
    {
        for (auto& mate : mates)
        {
            auto percent = chanceDist(generator);
            if (percent < Cell::reproduce_chance)
            {
                children.push_back(std::make_shared<Cell>(Cell(*this, *mate)));
            }
        }
    }
    // Interaction finished
    mates.clear();
    neighbors = 0;

    return children;
}
