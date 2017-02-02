
#include "board.hpp"

namespace objects
{

const float Board::move_amount = 25.0f;
const float Board::circle_size = 10.0f;
const int Board::logging_timesteps = 250;


Board::Board(int width, int height) :
    window(sf::VideoMode(width, height), "Life"),
    averagePoint(circle_size + 2)
{
    border.setFillColor(sf::Color(0, 0, 0, 0));
    border.setOutlineColor(sf::Color(200, 0, 200, 128));
    border.setOutlineThickness(10.0);
    
    auto view = window.getView();
    simulation_view = view;
    info_view = view;
    
    if (!font.loadFromFile("resources/UbuntuMono-R.ttf"))
    {
        std::cerr << "Failed to load font: Was the program run from the openlife directory?" << std::endl;
    }
    info.setFont(font);
    info.setCharacterSize(24);

    averagePoint.setFillColor(sf::Color(255, 102, 0));
    averagePoint.setOrigin(sf::Vector2f(circle_size + 2, circle_size + 2));

    search_radius.setFillColor(sf::Color(0, 0, 0, 0));
    search_radius.setOutlineColor(sf::Color(0, 0, 255, 128));
    search_radius.setOutlineThickness(2.0);
    neighbor_radius.setFillColor(sf::Color(0, 0, 0, 0));
    neighbor_radius.setOutlineColor(sf::Color(255, 0, 0, 128));
    neighbor_radius.setOutlineThickness(2.0);
    mate_radius.setFillColor(sf::Color(0, 0, 0, 0));
    mate_radius.setOutlineColor(sf::Color(0, 255, 0, 128));
    mate_radius.setOutlineThickness(2.0);
}


void Board::_update()
{
    using std::to_string;
    simulation.update();

    auto size       = sqrt(simulation.getArea()) / 2.0;
    auto averageLoc = averagePoint.getPosition();
    auto x_dev      = averageLoc.x - size;
    auto y_dev      = averageLoc.y - size;

    double average_rotation = 0;
    for (auto cell : simulation.cells)
    {
        average_rotation += cell->getRotation();
    }
    average_rotation /= simulation.cells.size();

    info.setString("Cells: "   + to_string(simulation.getCellCount()) + 
                  "\nDensity: " + to_string(simulation.getCellCount() * 1000000 / simulation.getArea()) + 
                  "\nAverage center deviation: " + to_string(x_dev) + ", " + to_string(y_dev) + 
                  "\nAverage rotation: " + to_string(average_rotation) + 
                  "\nDrawtime: " + to_string(frame_time/1000.) + "ms");
}

void Board::_render(bool debug)
{
    window.clear();

    window.setView(simulation_view);

    // Simulation
    _drawSimulation(debug);

    // Gui/Info
    window.setView(info_view);

    window.draw(info);

    window.display(); // For organization
}

void Board::_drawSimulation(bool debug)
{
    
    float averageLocation [2];
    averageLocation[0] = 0.;
    averageLocation[1] = 0.;
    
    sf::CircleShape cellCircle = sf::CircleShape(circle_size);
    cellCircle.setOrigin(sf::Vector2f(circle_size, circle_size));
    // Two loops so that cells get drawn above debug info
    // Debug info first
    for (auto cell : simulation.cells)
    {
        auto pos = cell->getPosition(); 
        if (debug)
        {
            search_radius.setPosition(pos.x, pos.y);
            window.draw(search_radius);
            mate_radius.setPosition(pos.x, pos.y);
            window.draw(mate_radius);
            neighbor_radius.setPosition(pos.x, pos.y);
            window.draw(neighbor_radius);
        }
    }
    // Cell bodies second
    for (auto cell : simulation.cells)
    {
        auto pos = cell->getPosition(); 
        cellCircle.setFillColor(sf::Color(cell->getApproximateFillColor()));
        cellCircle.setPosition((float) pos.x, (float) pos.y );
        window.draw(cellCircle);
        
        averageLocation[0] += (float) cell->getPosition().x;
        averageLocation[1] += (float) cell->getPosition().y;
    }

    averageLocation[0] = averageLocation[0] / simulation.cells.size();
    averageLocation[1] = averageLocation[1] / simulation.cells.size();
    
    averagePoint.setPosition(averageLocation[0] + circle_size/2, averageLocation[1] + circle_size/2); 
    
    window.draw(border);
    window.draw(averagePoint);
    
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
                Board::border.setOutlineThickness(Board::border.getOutlineThickness() * 0.95f);
            }
            else
            {
                simulation_view.zoom(1.05f);
                Board::border.setOutlineThickness(Board::border.getOutlineThickness() * 1.05f);
            }
        }
    }
}

void Board::_pan()
{
    const auto pressed = [](auto key){return sf::Keyboard::isKeyPressed(key);};

    if (pressed(sf::Keyboard::A)   ||
        pressed(sf::Keyboard::Left)||
        pressed(sf::Keyboard::H))
        simulation_view.move(-Board::move_amount, 0.0f);
    if (pressed(sf::Keyboard::S)   ||
        pressed(sf::Keyboard::Down)||
        pressed(sf::Keyboard::J))
        simulation_view.move(0.0f, Board::move_amount);
    if (pressed(sf::Keyboard::W) ||
        pressed(sf::Keyboard::Up)||
        pressed(sf::Keyboard::K))
        simulation_view.move(0.0f, -Board::move_amount);
    if (pressed(sf::Keyboard::D)    ||
        pressed(sf::Keyboard::Right)||
        pressed(sf::Keyboard::L))
        simulation_view.move(Board::move_amount, 0.0f);
}

// This function should really only call other functions (Similar to how no code goes in int main)


void Board::run(int nCells, int x, int y, bool debug) 
{
    simulation = Simulation(nCells, (double) x, (double) y);
    Board::border = sf::RectangleShape(sf::Vector2f(x, y));
    Board::border.setFillColor(sf::Color(0x00000000));
    Board::border.setOutlineColor(sf::Color(200, 0, 200, 128));
    Board::border.setOutlineThickness(4.0);

    double radius;
    radius = simulation.get_param("search_radius");
    search_radius.setRadius(radius);
    search_radius.setOrigin(sf::Vector2f(radius, radius));
    radius = simulation.get_param("neighbor_radius");
    neighbor_radius.setRadius(radius);
    neighbor_radius.setOrigin(sf::Vector2f(radius, radius));
    radius = simulation.get_param("mate_radius");
    mate_radius.setRadius(radius);
    mate_radius.setOrigin(sf::Vector2f(radius, radius));
    
    unsigned long long logTime = getTime();
    
    while (window.isOpen())
    {
        unsigned long long start_frame = getTime();
        _handle();
        _update();
        _render(debug);

        timestepsCompleted++;

        if (simulation.getCellCount() == 0) break;
        frame_time = getTime() - start_frame;
        
        if (frame_time < 1000000/60) {
            usleep((int) (1000000/60) - frame_time);
        }
        
        

        if (Board::timestepsCompleted % Board::logging_timesteps == 0){
            
            statistics s = simulation.getStatistics();
            
            std::cout << "Timestep: " << Board::timestepsCompleted << std::endl;
            std::cout << "Processed " << Board::logging_timesteps << " steps in " << std::to_string((getTime() - logTime)/1000) << "ms" << std::endl;
            std::cout << "Alive: " << simulation.getCellCount() << " total births: " << s.births << " total deaths: " << s.deaths << std::endl;
            std::cout << "Overpopulation deaths: " << s.overpopdeaths << " underpopulation deaths: " << s.underpopdeaths << std::endl;
            logTime = getTime();
        }
        
        
    }
    
    
}

} 
