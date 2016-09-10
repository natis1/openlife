#include "board.hpp"

Board::Board(int width, int height) :
    window(sf::VideoMode(width, height), "")
{
}

bool Board::_inBounds(Cell& cell)
{
    auto pos    = cell.getPosition();
    auto radius = cell.getRadius();
    auto size   = window.getSize();
    // Addition of radius will make the cells bounce when their edges touch the window's edges
    return pos.x - radius > 0      and 
           pos.x + radius < size.x and 
           pos.y - radius > 0      and 
           pos.y + radius < size.y;
}

// Randomly generate n cells
void Board::_genCells(int nCells)
{
    for (unsigned i = 0; i < nCells; i++)
    {
        _genCell();
    }
}

// Randomly generate a single cell
void Board::_genCell()
{
    auto cell = std::make_shared<Cell>(Cell());

    auto radius = cell->getRadius();
    auto size   = window.getSize();

    // Distributions for initial random settings
    auto widthDist  = dist(radius, size.x - radius);
    auto heightDist = dist(radius, size.y - radius);
    auto angleDist  = dist(0, 360);
    auto redDist    = dist(100, 255);
    auto blueDist   = dist(100, 255);

    // An alternative to providing a seed
    auto generator  = randomGenerator();

    int x = widthDist(generator);
    int y = heightDist(generator);

    cell->setPosition(x, y);
    cell->setRotation(angleDist(generator));
    // Random shade of purple or pink
    cell->setFillColor(sf::Color(redDist(generator), 0, blueDist(generator)));

    cells.push_back(cell);
}


void Board::_updateInteractions()
{
    CellVec remaining;
    remaining.reserve(cells.size()); // No allocation problems :)
    // ~Efficiently build the list of neighbors/mates
    // This technically runs in O(n + E(1, n){n - i}) (E(){} is meant to be a summation)
    // I've chosen to use iterators because it makes slicing possible, which is where the efficiency gains come from
    auto it = cells.begin();
    while (it != cells.end())
    {
        auto cell = *it;
        remaining = CellVec(it + 1, cells.end()); // Slice off the first element of the vector

        cell->interact(remaining);
        if (not _inBounds(**it))
        {
            // Modify the cell to push it into bounds
            cell->bounce();
        }
        it++;
    }
}

void Board::_update()
{
    auto size       = window.getSize();
    auto widthDist  = dist(0, size.x);
    auto heightDist = dist(0, size.y);
    auto generator  = randomGenerator();

    _updateInteractions();

    // Interact with neighbors, mate with mates
    std::vector<std::shared_ptr<Cell>> new_generation;
    for (auto cell : cells)
    {
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

void Board::_render()
{
    window.clear();
    for (auto cell : cells)
    {
        window.draw(*cell);
    }
    window.display();
}

void Board::_handle()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}

void Board::run(int nCells)
{
    _genCells(nCells);

    while (window.isOpen())
    {
        _handle();
        _update();
        _render();

        if (cells.size() == 0)
        {
            break;
        }
    }
}

