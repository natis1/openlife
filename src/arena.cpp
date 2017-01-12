#include "arena.hpp"

namespace objects
{


Arena::Arena()
{
//     auto view = window.getView();
//     simulation_view = view;
//     info_view = view;
/*
    if (!font.loadFromFile("resources/UbuntuMono-R.ttf"))
    {
        std::cout << "Failed to load font: Was the program run from the openlife directory?" << std::endl;
    }*/
//     info.setFont(font);
//     info.setCharacterSize(24);
// 
//     frame_display.setFont(font);
//     frame_display.setCharacterSize(24);
//     frame_display.setPosition(0., 40.);
}


void Arena::_update()
{
    simulation.update();
//     info.setString("Cells: " + std::to_string(simulation.getCellCount()) + " Density: " + (std::to_string(simulation.getCellCount() * 1000000 / simulation.getArea())));
//     frame_display.setString( "Drawtime: " + std::to_string(frame_time) + "us");
}
/*
void Board::_render()
{
    window.clear();

    window.setView(simulation_view);

    // Simulation
    simulation.render(window);

    // Gui/Info
    window.setView(info_view);

    window.draw(info);
    window.draw(frame_display);

    window.display(); // For organization
}

void Board::_handle()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        _pan();
        _zoom(event);
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                {
                sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
                simulation_view.reset(visible_area);
                info_view.reset(visible_area);
                }
                break;
            default:
                break;
        };
    }
}

void Board::_zoom(sf::Event& event)
{
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        {
            int amount = event.mouseWheelScroll.delta;
            if (amount == 1)
            {
                simulation_view.zoom(0.95f);
            }
            else
            {
                simulation_view.zoom(1.05f);
            }
        }
    }
}

void Board::_pan()
{
    const auto pressed = [](auto key){return sf::Keyboard::isKeyPressed(key);};

    if (pressed(sf::Keyboard::A) ||
        pressed(sf::Keyboard::Left))
        simulation_view.move(-Board::move_amount, 0.0f);
    if (pressed(sf::Keyboard::S) ||
        pressed(sf::Keyboard::Down))
        simulation_view.move(0.0f, Board::move_amount);
    if (pressed(sf::Keyboard::W) ||
        pressed(sf::Keyboard::Up))
        simulation_view.move(0.0f, -Board::move_amount);
    if (pressed(sf::Keyboard::D) ||
        pressed(sf::Keyboard::Right))
        simulation_view.move(Board::move_amount, 0.0f);
}
*/

// This function should really only call other functions (Similar to how no code goes in int main)
void Arena::run(int nCells, int x, int y)
{
    using tools::getTime;

    //auto spawn_area = sf::RectangleShape(sf::Vector2f(x/6, y));
    simulation = Simulation(nCells, x, y);

    //auto view_port = sf::FloatRect(0, 0, x, y);
    //simulation_view.reset(view_port);

    while (simulationRunning)
    {
        unsigned long long start_frame = getTime();
        //_handle();
        _update();
        //_render();

        if (simulation.getCellCount() == 0) break;
        frame_time = getTime() - start_frame;
    }
}

}
