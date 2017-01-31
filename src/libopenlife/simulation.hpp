#include "cell.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include "paramdict.hpp"

struct rectangleShape {
    double x = 0;
    double y = 0;
    double width;
    double height;
};

struct statistics {
    int births = 0;
    int deaths = 0;
    int overpopdeaths = 0;
    int underpopdeaths = 0;
    
};


using tools::print;
using tools::getTime;

// Code for simulation backend only
struct Simulation
{
    const static int csv_save_period;

    Simulation();
    Simulation(int nCells, double width, double height);
    Simulation(std::vector<std::shared_ptr<Cell>> inputCells, double width, double height);
    Simulation(int nCells, double width, double height, int spawnXSize, int spawnYSize);
    
    //~Simulation();
    statistics getStatistics();
    

    void update();
    void updateInteractions();
    
    std::vector<std::shared_ptr<Cell>> cells;

    int getCellCount();
    float getArea();
    
    position center_marker;

    double get_param(std::string param);

private:
    // Functions for generating random cells    
    void _generateManyRandomCells(int nCells, int spawnXSize, int spawnYSize);
    
    
    bool _inBounds(Cell& cell);
    void _updateInteractions();
    position _getAverageLocation();
    
    rectangleShape border;
    statistics simulationStatus;

    unsigned long long last_update;
    int update_count;

    ParamDict simulation_params;
};
