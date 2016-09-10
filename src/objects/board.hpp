#pragma once
#include "cell.hpp"

// To keep constant frametime
#include <sys/time.h>
#include <unistd.h>

class Board
{
public:
    Board(int width=640, int height=480);
    void run(int nCells=100);

private:
    // Helper functions / Internal methods
    
    // Functions for generating random cells
    void _genNear(Cell parent, int tolerance=10);
    void _genCells(int nCells);
    std::shared_ptr<Cell> _generateRandomCell();
    
    bool _inBounds(Cell& cell);

    void _updateInteractions();
    void _update();
    void _render();
    void _handle();
    
    unsigned long long _getExecutionTime();

    // Graphics frontend
    sf::RenderWindow window;
    sf::Font font;
    sf::Text info;
    sf::Text frameDisplay;

    // Simulation backend
    std::vector<std::shared_ptr<Cell>> cells;
};
