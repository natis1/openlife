#include "cell.hpp"

const float Cell::mate_radius      = 70.0f;
const float Cell::neighbor_radius  = 100.0f;
const float Cell::search_radius    = 400.0f;
const float Cell::move_modifier    = 1.5f;

const int Cell::underpopulation_limit = 2;
const int Cell::overpopulation_limit  = 14;
const int Cell::crowded_number = 10;

const double Cell::regeneration_amount    = 0;//0.01;
const double Cell::underpopulation_damage = 1.;
const double Cell::overpopulation_damage  = 1.;
const double Cell::affection_threshold = 100.;
const double Cell::turn_rate = 1.5; // Degrees
const double Cell::max_life  = 500.0;

int Cell::overpopulation_deaths  = 0;
int Cell::underpopulation_deaths = 0;
int Cell::births                 = 0;

// Create initial cell
Cell::Cell() :
    Entity(10, Cell::max_life),
    genome()
{
}

// Create child cell
Cell::Cell(Cell& a, Cell& b) :
    Cell::Cell()
{
    using tools::avg;

    genome = Genome(a.genome, b.genome);

    setRotation(a.getRotation() + 90.);

    position posA = a.getPosition();
    position posB = b.getPosition();

    // Spawn location for child cells
    auto dx = avg(posA.x, posB.x);
    auto dy = avg(posA.y, posB.y);

    setPosition(dx, dy);
    setFillColor(genome.representation());
    Cell::births++;
}

void Cell::bounce(double wallx, double wally, double wallwidth, double wallheight)
{
    auto rotation = getRotation();
    auto radius   = getRadius();
    auto pos      = getPosition();

    double normal = 0; // Angle normal to surface
    double reflected = 0; // Angle after reflection off of surface

    bool left   = pos.x - radius < wallx;
    bool right  = pos.x + radius > wallx + wallwidth;
    bool top    = pos.y - radius < wally;
    bool bottom = pos.y + radius > wally + wallheight;

    // Corners don't use a normal angle because it doesn't make sense (collision is with two surfaces)
    // Single-plane collisions use the law of reflection

    // Corner cases -> Turn 180 degrees (Other methods, like facing the center of the board, resulted in bugs)
    if (left && top)
    {
        reflected = (int)(rotation + 180.) % 360;
        move(1.0, 1.0);
    }
    else if (left && bottom)
    {
        move(1.0, -1.0);
        reflected = (int)(rotation + 180) % 360;
    }
    else if (right && top)
    {
        move(-1.0, 1.0);
        reflected = (int)(rotation + 180) % 360;
    }
    else if (right && bottom)
    {
        move(-1.0, -1.0);
        reflected = (int)(rotation + 180) % 360;
    }

    // Edge cases
    else
    {
        if (left)       
        {
            normal = 0;
            move(1.0, 0);
        }
        else if (right) 
        {
            normal = 180;
            move(-1.0, 0);
        }
        else if (top) 
        {
            normal = 90;
            move(0, 1.0);
        }
        else if (bottom) 
        {
            normal = 270;
            move(0, -1.0);
        }

        float inverse_normal = (int)(normal + 180) % 360;
        float incidence      = rotation - inverse_normal;

        reflected = normal - incidence;
    }

    setRotation(reflected);
}


// Build a list/count of neighbors and mates
void Cell::interact(const std::vector<std::shared_ptr<Cell>>& cells)
{
    for (auto& cell : cells)
    {
        auto dist   = distance(*this, *cell);

        // Mate with closer cells, treat further ones as neighbors, and ignore the rest
        if (dist < Cell::mate_radius)
        {
            mates.push_back(cell);
        }

        if (dist < Cell::neighbor_radius)
        {
            addNeighbor(cell);
            cell->addNeighbor(std::make_shared<Cell>(*this));
        }

        if (dist < Cell::search_radius)
        {
            addVisible(cell);
            cell->addVisible(std::make_shared<Cell>(*this));
        }
    }
}

void Cell::intelligentRotate(bool overpopulated)
{
    // If a cell has no neighbors, rotate towards visible cells. Otherwise, use neighborhood's center of mass
    auto center_of_mass = neighbors.empty() ? getAverageLocation(visible)
                                            : getAverageLocation(neighbors);

    double ideal_angle = angle(center_of_mass, getPosition());
    double current_angle = getRotation();
    if (overpopulated) ideal_angle = remainder(ideal_angle + 180., 360.);

    if (ideal_angle > current_angle + turn_rate) {
        rotate(turn_rate);
    } else if (ideal_angle < current_angle - turn_rate) {
        rotate(-turn_rate);
    } else {
        rotate(ideal_angle);
    }
}

position getAverageLocation(std::vector<std::shared_ptr<Cell>> cells)
{
    position averagePoint = {0, 0};
    if (cells.size() == 0)
    {
        return averagePoint;
    }
    for (auto& cell : cells) 
    {
        averagePoint.x += cell->getPosition().x;
        averagePoint.y += cell->getPosition().y;
    }

    averagePoint.x /= cells.size();
    averagePoint.y /= cells.size();

    return averagePoint;
}

void Cell::addVisible (std::shared_ptr<Cell> cell)
{
    visible.push_back(cell);
}

void Cell::addNeighbor(std::shared_ptr<Cell> cell)
{
    neighbors.push_back(cell);
}

void Cell::update()
{
    auto radius = getRadius();

    // Declare boolean values to make code more understandable
    bool underpopulated = neighbors.size() < Cell::underpopulation_limit;
    bool overpopulated  = neighbors.size() > Cell::overpopulation_limit;
    
    bool crowded = neighbors.size() > Cell::crowded_number;

    intelligentRotate(crowded);
    moveVec(*this, Cell::move_modifier); 
    if (overpopulated)
    {
        damage(Cell::overpopulation_damage);
        overpopulation_occurances++;
    }
    else if (underpopulated)
    {
        damage(Cell::underpopulation_damage);
        underpopulation_occurances++;
    }
    else 
    {
        // Begin mating if in appropriate conditions
        affection += genome.gene("affection_prime");
        //print("Affection increased to " + std::to_string(affection));
        regen(Cell::regeneration_amount);
    }
    if (not alive())
    {
        if (overpopulation_occurances > underpopulation_occurances)
        {
            Cell::overpopulation_deaths++;
        }
        else
        {
            Cell::underpopulation_deaths++;
        }
    }
    // Fade on death
    setFillColor(getFillColor().r, getFillColor().g, getFillColor().b, 255 * lifePercent());
}

std::string Cell::csv()
{
    auto position = getPosition();
    return std::to_string(position.x) + "," + std::to_string(position.y);
}

std::vector<std::shared_ptr<Cell>> Cell::mate()
{
    std::vector<std::shared_ptr<Cell>> children;

    if (mates.size() > 0)
    {
        for (auto& mate : mates)
        {
            if (affection + mate->affection > Cell::affection_threshold)
            {
                affection = 0;
                mate->affection = 0;
                children.push_back(std::make_shared<Cell>(Cell(*this, *mate)));
            }
        }
    }

    // Interaction finished
    mates.clear();
    neighbors.clear();
    visible.clear();

    return children;
}

std::tuple<int, int, int> Cell::getCellStatistics()
{
    return std::make_tuple(Cell::births,
                           Cell::overpopulation_deaths,
                           Cell::underpopulation_deaths);
}

