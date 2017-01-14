#include "simulation.hpp"
#include <algorithm>

const int Simulation::csv_save_period = 500000;

Simulation::Simulation(){}

Simulation::Simulation(int nCells, double width, double height)
{
    //spawn_area = set_spawn_area;
    
    std::cout << "creating Simulation" << std::endl;
    
    border.x = 0.; border.y = 0.;
    border.width = width;
    border.height = height;

    _genCells(nCells);
    last_update = getTime();
    update_count = 0;
}

Simulation::~Simulation()
{
    auto statistics = Cell::getCellStatistics();
    print("Statistics from simulation:");
    print("Births:                 " + std::to_string(std::get<0>(statistics)));
    print("Overpopulation deaths:  " + std::to_string(std::get<1>(statistics)));
    print("Underpopulation deaths: " + std::to_string(std::get<2>(statistics)));
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
            cell->bounce(border.x, border.y, border.width, border.height);
        }
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
    remaining.reserve(cells.size()); // speeds up allocation :)
    /* ~Efficiently build the list of neighbors/mates
    
    
    If you simplify O(n + E(1, n, n-1)), it becomes O((n / 2) * n), which is O(n^2)
    What I actually meant is that this algorithm is ~more~ AS efficient AS T = n^2:
    
    
    Values / Instructions / n^2
    1      / 1            / 1
    2      / 3            / 4
    3      / 6            / 9
    4      / 10           / 16
    ...
    10     / 55           / 100
    100    / 5050         / 10000
    1000   / 500500       / 1000000
    Difference between any value: n^2 - (n-1)^2
    Thereby these two functions are basically as efficient even if one is twice as fast.
    
    
    http://stackoverflow.com/questions/11032015/how-to-find-time-complexity-of-an-algorithm
    
    I've chosen to use iterators because it makes slicing possible, which is where the efficiency gains come from */
    auto it = cells.begin();
    while (it != cells.end())
    {
        auto cell = *it;
        remaining = std::vector<std::shared_ptr<Cell>>(it + 1, cells.end()); // Slice off the first element of the vector

        cell->interact(remaining);
        it++;
    }
}

bool Simulation::_inBounds(Cell& cell)
{
    // Checking x first. b.x < cell < x+width
    position cellloc = cell.getPosition();
    return ((border.x < cellloc.x && cellloc.x < (border.x + border.width))
        && (border.y < cellloc.y && cellloc.y < (border.y + border.height)));
}

// Randomly generate n cells
void Simulation::_genCells(int nCells)
{
    for (unsigned i = 0; i < nCells; i++)
    {
        // Generates a new cell randomly
        cells.push_back(_generateRandomCell());
    }
}

// Randomly generate a single cell
std::shared_ptr<Cell> Simulation::_generateRandomCell()
{
    // Explicitly import names
    using tools::dist;
    using tools::randomGenerator;

    std::shared_ptr<Cell> cell = std::make_shared<Cell>(Cell());

    //double radius = cell->getRadius();
    
    position size {border.width, border.height};
    // Distributions for initial random settings
    auto widthDist  = dist((int)border.x, (int)size.x);
    auto heightDist = dist((int)border.y, (int)size.y);
    auto angleDist  = dist(0, 360);

    // Mersenne Twister :)
    auto generator  = randomGenerator();

    int x = widthDist(generator);
    int y = heightDist(generator);

    cell->setPosition(x, y);
    cell->setRotation(angleDist(generator));
    
    return cell;
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
