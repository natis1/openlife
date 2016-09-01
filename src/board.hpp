#pragma once
#include "cell.hpp"
#include <vector>

class Board
{
public:
    Board(int width=640, int height=480);
    void run(int nCells=100);

private:
    // Helper functions / Internal methods
    void _genCell();
    void _genNear(Cell parent, int tolerance=10);
    void _genCells(int nCells);

    bool _inBounds(Cell& cell);

    // Graphics frontend
    sf::RenderWindow window;

    // Simulation backend
    std::vector<std::shared_ptr<Cell>> cells;
};
