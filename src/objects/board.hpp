#pragma once
#include "cell.hpp"
#include "../tools/tools.hpp"

namespace objects
{
class Board
{

    const static float move_amount;

public:
    Board(int width=640, int height=480);
    void run(int nCells=100, int x=1000, int y=1000);

private:
    // Helper functions / Internal methods
    
    // Functions for generating random cells
    void _genCells(int nCells);
    std::shared_ptr<Cell> _generateRandomCell();
    
    bool _inBounds(Cell& cell);

    void _updateInteractions();
    void _update();
    void _render();
    void _handle();
    

    // Graphics frontend
    sf::RenderWindow window;
    sf::Font font;
    sf::Text info;
    sf::Text frame_display;

    sf::View cell_view;
    sf::View info_view;

    // Simulation backend
    std::vector<std::shared_ptr<Cell>> cells;
    sf::RectangleShape border;
};
}
