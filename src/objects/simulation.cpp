#include "simulation.hpp"

namespace objects
{

Simulation::Simulation(){}

Simulation::Simulation(int nCells, int width, int height)
{
    border = sf::RectangleShape(sf::Vector2f(width, height));
    border.setFillColor(sf::Color(0, 0, 0, 0));
    border.setOutlineColor(sf::Color(200, 0, 200, 128));
    border.setOutlineThickness(10.0);

    _genCells(nCells);
}

int Simulation::getCellCount()
{
    return cells.size();
}

void Simulation::render(sf::RenderWindow& target)
{
    for (auto cell : cells)
    {
        target.draw(*cell);
    }
    target.draw(border);
}

void Simulation::update()
{
    updateInteractions();

    // Interact with neighbors, mate with mates
    std::vector<std::shared_ptr<Cell>> new_generation;
    for (auto cell : cells)
    {
        if (not _inBounds(*cell))
        {
            // Modify the cell to push it into bounds
            cell->bounce(border.getSize());
        }
        cell->update();
        auto children = cell->mate(); // Produce children with current set of mates, then clear list of mates
        new_generation.insert(new_generation.end(), children.begin(), children.end());
    }
    cells.insert(cells.end(), new_generation.begin(), new_generation.end());
    // Efficiently remove dead cells
    cells.erase(
        std::remove_if(cells.begin(), cells.end(),
                    [](const auto & e) { return not e->alive(); }),
            cells.end());

}

void Simulation::updateInteractions()
{
    std::vector<std::shared_ptr<Cell>> remaining;
    remaining.reserve(cells.size()); // No allocation problems :)
    /* ~Efficiently build the list of neighbors/mates
    
    If you simplify O(n + E(1, n, n-1)), it becomes O((n / 2) * n), which is O(n^2)
    What I actually meant is that this algorithm is more efficient than T = n^2:
    
    Values / Instructions / n^2
    1      / 1            / 1
    2      / 3            / 4
    3      / 6            / 9
    4      / 10           / 16
    ...
    10     / 55           / 100
    100    / 5050         / 10000
    1000   / 500500       / 1000000
    
    http://stackoverflow.com/questions/11032015/how-to-find-time-complexity-of-an-algorithm
    
    I've chosen to use iterators because it makes slicing possible, which is where the efficiency gains come from */
    auto it = cells.begin();
    while (it != cells.end())
    {
        auto cell = *it;
        remaining = std::vector<std::shared_ptr<Cell>>(it + 1, cells.end()); // Slice off the first element of the vector

        cell->interact(remaining);
        it++;
    }
}

bool Simulation::_inBounds(Cell& cell)
{
    return border.getGlobalBounds().contains(cell.getPosition());
}

// Randomly generate n cells
void Simulation::_genCells(int nCells)
{
    for (unsigned i = 0; i < nCells; i++)
    {
        // Generates a new cell randomly
        cells.push_back(_generateRandomCell());
    }
}

// Randomly generate a single cell
std::shared_ptr<Cell> Simulation::_generateRandomCell()
{
    // Explicitly import names
    using tools::dist;
    using tools::randomGenerator;

    std::shared_ptr<Cell> cell = std::make_shared<Cell>(Cell());

    auto radius = cell->getRadius();
    auto size   = border.getSize();
// Distributions for initial random settings
    auto widthDist  = dist(0, (int)size.x);
    auto heightDist = dist(0, (int)size.y);
    auto angleDist  = dist(0, 360);

    // An alternative to providing a seed
    auto generator  = randomGenerator();

    int x = widthDist(generator);
    int y = heightDist(generator);

    cell->setPosition(x, y);
    cell->setRotation(angleDist(generator));
    
    return cell;
}

}
