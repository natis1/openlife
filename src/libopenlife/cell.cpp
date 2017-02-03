#include "cell.hpp"

// Create initial cell
Cell::Cell(double size, double life) :
    Entity(size, life),
    genome()
{
}

// Create child cell
Cell::Cell(double size, double life, Cell& a, Cell& b) :
    Cell::Cell(size, life)
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

void Cell::bounce(double wallx, double wally, double wallwidth, double wallheight, double move_amount)
{
    auto rotation = getRotation();
    auto radius   = getRadius();
    auto pos      = getPosition();

    double normal = 0; // Angle normal to surface
    double reflected = modAngle(rotation + 180.); // Angle after reflection off of surface (default = 180)

    bool left   = pos.x - radius < wallx;
    bool right  = pos.x + radius > wallx + wallwidth;
    bool top    = pos.y - radius < wally;
    bool bottom = pos.y + radius > wally + wallheight;

    // Corners don't use a normal angle because it doesn't make sense (collision is with two surfaces)
    // Single-plane collisions use the law of reflection

    // Corner cases -> Turn 180 degrees (Other methods, like facing the center of the board, resulted in bugs)
    if (left && top)
    {
        move(move_amount, move_amount);
    }
    else if (left && bottom)
    {
        move(move_amount, -move_amount);
    }
    else if (right && top)
    {
        move(-move_amount, move_amount);
    }
    else if (right && bottom)
    {
        move(-move_amount, -move_amount);
    }

    // Edge cases
    else
    {
        if (left)       
        {
            normal = 0;
            move(move_amount, 0);
        }
        else if (right) 
        {
            normal = move_amount;
            move(-move_amount, 0);
        }
        else if (top) 
        {
            normal = 90;
            move(0, move_amount);
        }
        else if (bottom) 
        {
            normal = 270;
            move(0, -move_amount);
        }

        float inverse_normal = modAngle(normal + 180);
        float incidence      = modAngle(rotation - inverse_normal);

        reflected = modAngle(normal - incidence);
    }

    setRotation(reflected);
    moveVec(*this, move_amount);
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

void Cell::intelligentRotate(bool underpopulated, bool crowded, ParamDict& simulation_params)
{
    using tools::getLimitedAngle;
    // If a cell has no neighbors, rotate towards visible cells. Otherwise, use neighborhood's center of mass
    auto visible_center  = getAverageLocation(visible);
    auto neighbor_center = getAverageLocation(neighbors);
    auto center_of_mass  = neighbors.empty() ? visible_center :
                           // If crowded, turn away from midpoint between neighbor and visible centers
                           crowded           ? position{(visible_center.x + neighbor_center.x) / 2.0
                                                      , (visible_center.y + neighbor_center.y) / 2.0}
                                             : neighbor_center; 

    double ideal   = angle(getPosition(), center_of_mass);
    double current = getRotation();
    double limit   = simulation_params.get("turn_rate");

    if (crowded) ideal = modAngle(ideal + 180.);

    setRotation(getLimitedAngle(ideal, current, limit));
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
    bool underpopulated = neighbors.size() < (int)simulation_params.get("underpopulation_limit");
    bool overpopulated  = neighbors.size() > (int)simulation_params.get("overpopulation_limit");
    bool crowded        = neighbors.size() > (int)simulation_params.get("crowded_limit");

    intelligentRotate(underpopulated, crowded, simulation_params);
    //moveVec(*this, simulation_params.get("move_modifier")); 

    if (overpopulated)
    {
        auto overpop_damage = simulation_params.get("overpopulation_damage");
        damage(overpop_damage);
        overpopulation_damage_taken += (float) overpop_damage;        
    }
    else if (underpopulated)
    {
        auto underpop_damage = simulation_params.get("underpopulation_damage");
        damage(underpop_damage);
        underpopulation_damage_taken += (float) underpop_damage;
    }
    else 
    {
        affection += genome.gene("affection_prime");
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
                children.push_back(std::make_shared<Cell>(Cell(getRadius(), total_life, *this, *mate)));
            }
        }
    }

    // Interaction finished
    mates.clear();
    neighbors.clear();
    visible.clear();

    return children;
}



