#pragma once
#include "cell.hpp"

class Board
{
public:
    Board(int width=640, int height=480);
    void run(int nCells=100);

private:
    // Helper functions / Internal methods
    void _genCell();
    void _genNear(Cell parent, int tolerance=10);
    void _genCells();

    bool _inBounds(Cell& cell);

    void _updateNeighbors();
    void _update();
    void _render();
    void _handle();

    // Graphics frontend
    sf::RenderWindow window;

    // Simulation backend
    CellVec cells;

    int nCells;
};
