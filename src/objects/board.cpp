#include "board.hpp"

Board::Board(int width, int height) :
    window(sf::VideoMode(width, height), "")
{
}

bool Board::_inBounds(Cell& cell)
{
    auto pos  = cell.getPosition();
    auto size = window.getSize();
    return pos.x > 0 and pos.x < size.x and pos.y > 0 and pos.y < size.y;
}

void Board::_genCells()
{
    for (unsigned i = 0; i < nCells; i++)
    {
        _genCell();
    }
}

void Board::_genCell()
{
    auto size = window.getSize();

    // Distributions for initial random settings
    auto widthDist  = dist(0, size.x);
    auto heightDist = dist(0, size.y);
    auto angleDist  = dist(0, 360);
    auto redDist    = dist(100, 255);
    auto blueDist   = dist(100, 255);

    auto generator  = randomGenerator();

    int x = widthDist(generator);
    int y = heightDist(generator);

    auto cell = std::make_shared<Cell>(Cell());

    cell->setPosition(x, y);
    cell->setRotation(angleDist(generator));
    cell->setFillColor(sf::Color(redDist(generator), 0, blueDist(generator)));

    cells.push_back(cell);
}


void Board::_updateNeighbors()
{
    CellVec remaining;
    remaining.reserve(cells.size());
    // ~Efficiently build the list of neighbors/mates
    auto it = cells.begin();
    while (it != cells.end())
    {
        remaining = CellVec(it + 1, cells.end());
        (*it)->updateNeighbors(remaining);
        if (not _inBounds(**it))
        {
            (*it)->bounce();
            moveVec(**it, 1);
        }
        it++;
    }
}

void Board::_update()
{
    auto size = window.getSize();
    auto widthDist  = dist(0, size.x);
    auto heightDist = dist(0, size.y);
    auto generator  = randomGenerator();

    _updateNeighbors();

    // Interact with neighbors, mate with mates
    std::vector<std::shared_ptr<Cell>> new_generation;
    for (auto cell : cells)
    {
        cell->update();
        auto mates = cell->mate();
        new_generation.insert(new_generation.end(), mates.begin(), mates.end());
    }
    cells.insert(cells.end(), new_generation.begin(), new_generation.end());
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
    this->nCells = nCells;
    _genCells();

    while (window.isOpen())
    {
        _handle();
        _update();
        _render();

        if (cells.size() == 0)
        {
            std::cout << "No cells remaining" << std::endl;
            break;
        }
    }
}

