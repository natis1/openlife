#include "board.hpp"
#include <random>
#include <iostream>

Board::Board(int width, int height) :
    window(sf::VideoMode(width, height), "")
{
}

void Board::_genCells(int nCells)
{
    for (unsigned i; i < nCells; i++)
    {
        std::cout << "Generating cell" << std::endl;
        _genCell();
    }
    std::cout << "Done" << std::endl;
    std::cout << cells.size() << std::endl;
}

void Board::_genCell()
{
    Cell cell;
    cell.setFillColor(sf::Color(255, 0, 0));

    // Create and seed a random number generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    // Distributions for integer coordinates inside of the board
    std::uniform_int_distribution<std::mt19937::result_type> widthDist  (0, (int)window.getSize().x); 
    std::uniform_int_distribution<std::mt19937::result_type> heightDist (0, (int)window.getSize().y);

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

        window.clear();
        for (auto cell : cells)
        {
            window.draw(cell);
        }
        window.display();
    }
}

