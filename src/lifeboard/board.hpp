#pragma once
#include "../libopenlife/cell.hpp"
#include "../libopenlife/simulation.hpp"
#include "../libopenlife/tools.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace objects
{

class Board
{
    const static float move_amount;
    const static float circle_size;
    const static int logging_timesteps;

    sf::RectangleShape border;

public:
    Board(int width=640, int height=480);
    void run(int nCells=100, int x=1000, int y=1000);
    
private:
    // Helper functions / Internal methods
    
    long timestepsCompleted = 0;

    void _update();
    void _render();
    void _handle();
    void _drawSimulation();

    void _zoom(sf::Event& event);
    void _pan();

    // Graphics frontend
    sf::RenderWindow window;
    sf::Font font;
    sf::Text info;
    sf::Text frame_display;

    sf::View simulation_view;
    sf::View info_view;
    
    

    // Simulation backend
    Simulation simulation;

    // Timing
    unsigned long long frame_time;
};

}
