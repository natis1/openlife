#include "cell.hpp"

struct rectangleShape {
    double x = 0;
    double y = 0;
    double width;
    double height;
};


namespace objects
{

using tools::print;
using tools::getTime;

// Code for simulation backend only
struct Simulation
{
    const static int csv_save_period;

    Simulation();
    Simulation(int nCells, double width, double height);

    void update();
    void updateInteractions();
    
    //void render(sf::RenderWindow& target);
    
    std::vector<std::shared_ptr<Cell>> cells;

    int getCellCount();
    float getArea();
    
    position center_marker;

private:
    // Functions for generating random cells
    void _genCells(int nCells);
    std::shared_ptr<Cell> _generateRandomCell();
    
    bool _inBounds(Cell& cell);
    void _updateInteractions();
    position _getAverageLocation();
    
    rectangleShape border;

    unsigned long long last_update;
    int update_count;
    
    
    
};

}
