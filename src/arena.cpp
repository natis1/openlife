#include "arena.hpp"

namespace objects
{

const int Arena::logging_timesteps = 250;

Arena::Arena()
{
}


void Arena::_update()
{
    simulation.update();
}

// This function should really only call other functions (Similar to how no code goes in int main)
void Arena::run(int nCells, int x, int y)
{
    using tools::getTime;

    simulation = Simulation(nCells, x, y);
    unsigned long long start_frame = getTime();
    
    while (true)
    {
        _update();
        timestepsCompleted++;
        if (Arena::timestepsCompleted % Arena::logging_timesteps == 0){
            frame_time = getTime() - start_frame;
            frame_time /= 1000;
            
            statistics s = simulation.getStatistics();
            
            std::cout << "Timestep: " << Arena::timestepsCompleted << std::endl;
            std::cout << "Processed " << Arena::logging_timesteps << " steps in " << frame_time << "ms" << std::endl;
            std::cout << "Alive: " << simulation.getCellCount() << " total births: " << s.births << " total deaths: " << s.deaths << std::endl;
            std::cout << "Overpopulation deaths: " << s.overpopdeaths << " underpopulation deaths: " << s.underpopdeaths << std::endl;
            start_frame = getTime();
        }

        if (simulation.getCellCount() == 0) break;
        
    }
}


}
