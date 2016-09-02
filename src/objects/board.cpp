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

void Board::_genCells(int nCells)
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

void Board::_genFood(int nFood)
{
    auto size = window.getSize();
    auto widthDist  = dist(0, size.x);
    auto heightDist = dist(0, size.y);
    auto generator  = randomGenerator();

    for(unsigned i = 0; i < nFood; i++)
    {
        Food f;

        int x = widthDist(generator);
        int y = heightDist(generator);

        f.setPosition(x, y);
        f.setFillColor(sf::Color(0, 255, 255));

        food.push_back(std::make_shared<Food>(f));
    }
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
    cells.erase(
        std::remove_if(cells.begin(), cells.end(),
                    [](const auto & e) { return not e->alive(); }),
            cells.end());
    food.erase(
        std::remove_if(food.begin(), food.end(),
                    [](const auto & f) { return not f->alive(); }),
            food.end());

    _updateNeighbors();

    // Interact with neighbors, mate with mates
    std::vector<std::shared_ptr<Cell>> new_generation;
    for (auto cell : cells)
    {
        cell->update(food);
        auto mates = cell->mate();
        new_generation.insert(new_generation.end(), mates.begin(), mates.end());
    }
    cells.insert(cells.end(), new_generation.begin(), new_generation.end());
}

void Board::_render()
{
    window.clear();
    for (auto f : food)
    {
        window.draw(*f);
    }
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

void Board::run(int nCells, int nFood)
{
    _genCells(nCells);
    _genFood(nFood);

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

