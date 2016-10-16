#include "cell.hpp"

namespace objects
{

// Code for simulation backend only
struct Simulation
{
    Simulation();
    Simulation(int nCells, int width, int height, sf::RectangleShape spawn_area);

    void update();
    void updateInteractions();
    void render(sf::RenderWindow& target);

    int getCellCount();

private:
    // Functions for generating random cells
    void _genCells(int nCells);
    std::shared_ptr<Cell> _generateRandomCell();
    
    bool _inBounds(Cell& cell);
    void _updateInteractions();

    // Simulation backend
    std::vector<std::shared_ptr<Cell>> cells;
    sf::RectangleShape border;
    sf::RectangleShape spawn_area;
    sf::CircleShape    center_marker;
};

}
