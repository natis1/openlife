#include "simulation.hpp"
#include <algorithm>


const int Simulation::csv_save_period = 500000;

Simulation::Simulation(){}

Simulation::Simulation(int nCells, double width, double height, const ParamDict& set_params)
{
    simulation_params = set_params;
    
    border.x = 0.; border.y = 0.;
    border.width = width;
    border.height = height;
    
    // Different starting configurations based on params
    bool random = simulation_params.get("random_start") > .5;
    if (random)
    {
        _generateManyRandomCells(nCells, (int) width, (int) height);
    }
    else
    {
        auto size = simulation_params.get("cell_size");
        auto life = simulation_params.get("cell_life");
        Cell a(size, life);
        Cell b(size, life);
        a.setPosition(width/2. + 100., height/2.);
        b.setPosition(width/2. - 100., height/2.);
        cells.push_back(std::make_shared<Cell>(a));
        cells.push_back(std::make_shared<Cell>(b));
    }
    last_update = getTime();
    update_count = 0;
}

statistics Simulation::getStatistics()
{
    return simulationStatus;
}

int Simulation::getCellCount()
{
    return cells.size();
}

float Simulation::getArea()
{
    return border.width * border.height;
}

void Simulation::update()
{
    updateInteractions();
    
    // Interact with neighbors, mate with mates
    std::vector<std::shared_ptr<Cell>> new_generation;
    for (auto cell : cells)
    {
        if (not _inBounds(*cell))
        {
            // Modify the cell to push it into bounds
            //cell->bounce(border.x, border.y, border.width, border.height, 
            //             simulation_params.get("move_modifier"));
            
            cell->damage(10000.);
            
            
        }
        cell->update(simulation_params);
        auto children = cell->mate(simulation_params); // Produce children with current set of mates, then clear list of mates
        
        //Save 16 bytes in Cell() by storing births and deaths in Simulation instead.
        simulationStatus.births += children.size();
        if (!cell->alive()) {
            simulationStatus.deaths++;
            if (cell->overpopulation_damage_taken > cell->underpopulation_damage_taken) {
                simulationStatus.overpopdeaths++;
            } else {
                simulationStatus.underpopdeaths++;
            }
        }
        
        new_generation.insert(new_generation.end(), children.begin(), children.end());
    }
    cells.insert(cells.end(), new_generation.begin(), new_generation.end());
    // Efficiently remove dead cells
    cells.erase(
        std::remove_if(cells.begin(), cells.end(),
                    [](const auto & e) { return not e->alive(); }),
            cells.end());

    center_marker = getAverageLocation(cells);
    unsigned long long time_diff = getTime() - last_update;
    if (time_diff > Simulation::csv_save_period)
    {
        std::string count = std::to_string(update_count);
        for (int i = count.size(); i < 5; i++)
        {
            count = "0" + count;
        }
        std::string filename = "data/simulation_" + count + ".csv";
        //print(filename);
        tools::writeCSV(filename, cells);
        last_update = getTime();
        update_count++;
    }
}

void Simulation::updateInteractions()
{
    std::vector<std::shared_ptr<Cell>> remaining;
    remaining.reserve(cells.size()); 
    
    /*
     * TODO: Figure out if there's any good way to optimize this/ add multithreading
     */
    
    for (auto it = cells.begin(); it != cells.end(); it++)
    {
        auto cell = *it;
        remaining = std::vector<std::shared_ptr<Cell>>(it + 1, cells.end()); // Slice off the first element of the vector
        cell->interact(remaining, simulation_params);
    }
    for (auto& cell : cells)
    {
        moveVec(*cell, simulation_params.get("move_modifier"));
    }
}

bool Simulation::_inBounds(Cell& cell)
{
    // Checking x first. b.x < cell < x+width
    position cellloc = cell.getPosition();
    return ((border.x < cellloc.x && cellloc.x < (border.x + border.width))
        && (border.y < cellloc.y && cellloc.y < (border.y + border.height)));
}


void Simulation::_generateManyRandomCells(int nCells, int spawnXSize, int spawnYSize)
{
    
    using tools::dist;
    using tools::randomGenerator;
    
    
    // Distributions for initial random settings
    auto widthDist  = dist((int)border.x, spawnXSize - (int) border.x);
    auto heightDist = dist((int)border.y, spawnYSize - (int) border.y);
    auto angleDist  = dist(0, 360);
    
    
    // Use generation method specified in tools - Default: Mersenne Twister
    auto generator  = randomGenerator();
    
    cells.reserve(nCells);
    
    
    for (int i = 0; i < nCells; i++)
    {
        auto cell = std::make_shared<Cell>(Cell(simulation_params.get("cell_size"), simulation_params.get("cell_life")));
        cell->setPosition(widthDist(generator), heightDist(generator));
        cell->setRotation(angleDist(generator));
        cells.push_back(cell);
    }
}


position  Simulation::_getAverageLocation(){
    double x = 0.;
    double y = 0.;
    
    for (auto cell : cells)
    {
        x += cell->getPosition().x;
        y += cell->getPosition().y;
    }
    x /= cells.size();
    y /= cells.size();
    
    return {x, y};
}


