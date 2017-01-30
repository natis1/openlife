#include "cell.hpp"

// Create initial cell
Cell::Cell() :
    Entity(10, 500.),
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
void Cell::interact(const std::vector<std::shared_ptr<Cell>>& cells, ParamDict& simulation_params)
{
    for (auto& cell : cells)
    {
        auto dist   = distance(*this, *cell);

        if (dist < simulation_params.get("mate_radius"))
        {
            mates.push_back(cell);
        }

        if (dist < simulation_params.get("neighbor_radius"))
        {
            addNeighbor(cell);
            cell->addNeighbor(std::make_shared<Cell>(*this));
        }

        if (dist < simulation_params.get("search_radius"))
        {
            addVisible(cell);
            cell->addVisible(std::make_shared<Cell>(*this));
        }
    }
}

void Cell::intelligentRotate(bool overpopulated, ParamDict& simulation_params)
{
    // If a cell has no neighbors, rotate towards visible cells. Otherwise, use neighborhood's center of mass
    //auto center_of_mass = neighbors.empty() ? getAverageLocation(visible)
    //                                        : getAverageLocation(neighbors);
    // Alternatively, just rotate based on visible cells always
    auto center_of_mass = getAverageLocation(visible);

    double ideal_angle = angle(getPosition(), center_of_mass);
    double current_angle = getRotation();
    if (overpopulated) ideal_angle = modAngle(ideal_angle + 180.);

    auto turn_rate = simulation_params.get("turn_rate");

    rotate(tools::getTurnAngle(ideal_angle, current_angle, turn_rate));
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

void Cell::update(ParamDict& simulation_params)
{
    auto radius = getRadius();

    // Declare boolean values to make code more understandable
    bool underpopulated = neighbors.size() < (int)simulation_params.get("underpopulation_limit");
    bool overpopulated  = neighbors.size() > (int)simulation_params.get("overpopulation_limit");
    
    bool crowded = neighbors.size() > (int)simulation_params.get("crowded_limit");

    intelligentRotate(crowded, simulation_params);
    moveVec(*this, simulation_params.get("move_modifier")); 
    if (overpopulated)
    {
        damage(simulation_params.get("overpopulation_damage"));
        overpopulation_damage_taken += (float) simulation_params.get("overpopulation_damage");        
    }
    else if (underpopulated)
    {
        damage(simulation_params.get("underpopulation_damage"));
        underpopulation_damage_taken += (float) simulation_params.get("underpopulation_damage");
    }
    else 
    {
        // Begin mating if in appropriate conditions
        affection += genome.gene("affection_prime");
        //print("Affection increased to " + std::to_string(affection));
        regen(simulation_params.get("regeneration_amount"));
    }
    setFillColor(getFillColor().r, getFillColor().g, getFillColor().b, 255 * lifePercent());
}

std::string Cell::csv()
{
    auto position = getPosition();
    return std::to_string(position.x) + "," + std::to_string(position.y);
}

std::vector<std::shared_ptr<Cell>> Cell::mate(ParamDict& simulation_params)
{
    std::vector<std::shared_ptr<Cell>> children;

    if (mates.size() > 0)
    {
        for (auto& mate : mates)
        {
            if (affection + mate->affection > simulation_params.get("affection_threshold"))
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



