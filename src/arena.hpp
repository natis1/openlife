#pragma once
#include "libopenlife/cell.hpp"
#include "libopenlife/simulation.hpp"
#include "libopenlife/tools.hpp"

/*
 * The Arena file is used for openlife cli
 * It is very similar to the board file but with all graphics stripped out.
 * 
 */

namespace objects
{

class Arena
{

public:
    Arena();
    void run(int nCells=100, int x=1000, int y=1000, std::string paramFile="params.txt");

private:
    const static int logging_timesteps;
    long timestepsCompleted = 0;
    
    // Helper functions / Internal methods
    void _update();
    Simulation simulation;

    // Timing
    unsigned long long frame_time;
    
    bool simulationRunning = true;
};

}
