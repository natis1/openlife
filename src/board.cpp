#include "board.hpp"
#include <random>
#include <iostream>

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
    for (unsigned i; i < nCells; i++)
    {
        _genCell();
    }
}

void Board::_genCell()
{
    Cell cell;
    cell.setFillColor(sf::Color(255, 0, 0));

    // Create and seed a random number generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    // Distributions for integer coordinates inside of the board
    auto size = window.getSize();
    std::uniform_int_distribution<std::mt19937::result_type> widthDist  (0, (int)size.x); 
    std::uniform_int_distribution<std::mt19937::result_type> heightDist (0, (int)size.y);

    int x = widthDist(rng);
    int y = heightDist(rng);

    cell.setPosition(x, y);

    cells.push_back(cell);
}

void Board::run(int nCells)
{
    _genCells(nCells);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        for (auto& cell : cells)
        {
            cell.update();
            if (not _inBounds(cell))
            {
                cell.bounce();
            }
        }

        window.clear();
        for (auto cell : cells)
        {
            window.draw(cell);
        }
        window.display();
    }
}

