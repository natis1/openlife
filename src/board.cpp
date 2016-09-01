#include "board.hpp"
#include <random>
#include <iostream>
#include <memory>

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
    auto cell = std::make_shared<Cell>(Cell());

    // Create and seed a random number generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    // Distributions for integer coordinates inside of the board
    auto size = window.getSize();
    std::uniform_int_distribution<std::mt19937::result_type> widthDist  (0, (int)size.x); 
    std::uniform_int_distribution<std::mt19937::result_type> heightDist (0, (int)size.y);

    int x = widthDist(rng);
    int y = heightDist(rng);

    cell->setPosition(x, y);

    // Distributions for angles in degrees
    std::uniform_int_distribution<std::mt19937::result_type> angleDist  (0, 360); 
    // Coloration
    std::uniform_int_distribution<std::mt19937::result_type> colorDist  (0, 255); 

    cell->setRotation(angleDist(rng));
    cell->setFillColor(sf::Color(colorDist(rng), colorDist(rng), colorDist(rng)));

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

        std::vector<std::shared_ptr<Cell>> remaining;
        remaining.reserve(cells.size());

        // ~Efficiently build the list of neighbors/mates
        auto it = cells.begin();
        while (it != cells.end())
        {
            if (not (*it)->alive())
            {
                it = cells.erase(it);
            }
            else
            {
                remaining = std::vector<std::shared_ptr<Cell>>(it + 1, cells.end());
                (*it)->updateNeighbors(remaining);
                if (not _inBounds(**it))
                {
                    (*it)->bounce();
                }
                it++;
            }
        }

        // Interact with neighbors, mate with mates
        std::vector<std::shared_ptr<Cell>> new_generation;
        for (auto cell : cells)
        {
            cell->update();
            auto mates = cell->mate();
            new_generation.insert(new_generation.end(), mates.begin(), mates.end());
        }
        cells.insert(cells.end(), new_generation.begin(), new_generation.end());

        window.clear();
        for (auto cell : cells)
        {
            window.draw(*cell);
        }
        window.display();

        if (cells.size() == 0)
        {
            break;
        }
    }
}

