#include "board.hpp"

Board::Board(int width, int height) :
    window(sf::VideoMode(width, height), "")
{
    if (!font.loadFromFile("resources/UbuntuMono-R.ttf"))
    {
        std::cout << "Failed to load font: Was the program run from the openlife directory?" << std::endl;
    }
    info.setFont(font);
    info.setCharacterSize(24);
    
    frameDisplay.setFont(font);
    frameDisplay.setCharacterSize(24);
    frameDisplay.setPosition(0., 40.);
}

bool Board::_inBounds(Cell& cell)
{
    auto pos    = cell.getPosition();
    auto radius = cell.getRadius();
    auto size   = window.getSize();
    // Addition of radius will make the cells bounce when their edges touch the window's edges
    return pos.x - radius > 0      && 
           pos.x + radius < size.x && 
           pos.y - radius > 0      && 
           pos.y + radius < size.y;
}

// Randomly generate n cells
void Board::_genCells(int nCells)
{
    for (unsigned i = 0; i < nCells; i++)
    {
        // Generates a new cell randomly
        cells.push_back(_generateRandomCell());
    }
}

// Randomly generate a single cell
std::shared_ptr<Cell> Board::_generateRandomCell()
{
    std::shared_ptr<Cell> cell = std::make_shared<Cell>(Cell());

    auto radius = cell->getRadius();
    auto size   = window.getSize();

    // Distributions for initial random settings
    auto widthDist  = dist(2 * radius, size.x - (2 * radius));
    auto heightDist = dist(2 * radius, size.y - (2 * radius));
    auto angleDist  = dist(0, 360);
    auto redDist    = dist(0, 255);
    auto blueDist   = dist(0, 255);

    // An alternative to providing a seed
    auto generator  = randomGenerator();

    int x = widthDist(generator);
    int y = heightDist(generator);

    cell->setPosition(x, y);
    cell->setRotation(angleDist(generator));
    
    
    // Cells have 128 green so they can always be seen, red and blue go between 0 and 255 and represent turning rate and mating rate.
    
    // Higher mating rates mean more offspring but also more damage.
    cell->setFillColor(sf::Color(redDist(generator), 128, blueDist(generator)));

    return cell;
}


void Board::_updateInteractions()
{
    std::vector<std::shared_ptr<Cell>> remaining;
    auto size = window.getSize();
    remaining.reserve(cells.size()); // No allocation problems :)
    // ~Efficiently build the list of neighbors/mates
    // This runs in < O(n^2) from my testing. See for yourself, I added a frame timer.
    // I've chosen to use iterators because it makes slicing possible, which is where the efficiency gains come from
    auto it = cells.begin();
    while (it != cells.end())
    {
        auto cell = *it;
        remaining = std::vector<std::shared_ptr<Cell>>(it + 1, cells.end()); // Slice off the first element of the vector

        cell->interact(remaining);
        if (not _inBounds(**it))
        {
            // Modify the cell to push it into bounds
            cell->bounce(size);
        }
        it++;
    }
}

void Board::_update()
{
    auto size       = window.getSize();
    auto widthDist  = dist(0, size.x);
    auto heightDist = dist(0, size.y);
    auto generator  = randomGenerator();

    _updateInteractions();

    // Interact with neighbors, mate with mates
    std::vector<std::shared_ptr<Cell>> new_generation;
    for (auto cell : cells)
    {
        cell->update();
        auto children = cell->mate(); // Produce children with current set of mates, then clear list of mates
        new_generation.insert(new_generation.end(), children.begin(), children.end());
    }
    cells.insert(cells.end(), new_generation.begin(), new_generation.end());
    // Efficiently remove dead cells
    cells.erase(
        std::remove_if(cells.begin(), cells.end(),
                    [](const auto & e) { return not e->alive(); }),
            cells.end());

}

void Board::_render()
{
    window.clear();


    for (auto cell : cells)
    {
        window.draw(*cell);
    }

    info.setString("Cells: " + std::to_string(cells.size()));
    window.draw(info);
    
}

void Board::_handle()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}

unsigned long long Board::_getExecutionTime()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return now.tv_usec + (unsigned long long)now.tv_sec * 1000000;
}




void Board::run(int nCells)
{
    _genCells(nCells);

    int needInfo = 0;
    unsigned long long *frameTimeHistory [60];
    
    while (window.isOpen())
    {
        unsigned long long startFrame = _getExecutionTime();
        _handle();
        _update();
        _render();
        
        if (cells.size() == 0) break;
        unsigned long long frameTime = _getExecutionTime() - startFrame;
        //std::cout << frameTime << std::endl;
        
        frameTimeHistory[needInfo] = &frameTime;
        needInfo++;
        if (needInfo >= 59){
            unsigned long long average = 0;
            for (int i = 0; i < 60; i++) {
                average += *frameTimeHistory[i];
            }
            average = average / 60;
            
            frameDisplay.setString( "Drawtime: " + std::to_string(average) + "us");
            needInfo = 0;
        }
        window.draw(frameDisplay);
        window.display();
        
        // Assuming this math and cout takes 3 microsecond
        if (frameTime < 16664){
            usleep(16664 - frameTime);
        }
        
    }
}

