#pragma once
#include "cell.hpp"
#include "simulation.hpp"
#include "../tools/tools.hpp"

class Board
{
    const static float move_amount;

public:
    Board(int width=640, int height=480);
    void run(int nCells=100, int x=1000, int y=1000, bool debug=false, bool show_sim=true);

private:
    // Helper functions / Internal methods

    void _update();
    void _render(bool debug=false, bool show_sim=true);
    void _handle();

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

