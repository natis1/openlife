#include "cell.hpp"

// Create initial cell
Cell::Cell(double size, double life) :
    Entity(size, life)
{
}

// Create child cell
Cell::Cell(double size, double life, Cell& a, Cell& b) :
    Cell::Cell(size, life)
{
    using tools::avg;

    //setRotation(a.getRotation() + 90.);

    position posA = a.getPosition();
    position posB = b.getPosition();

    // Spawn location for child cells
    auto dx = avg(posA.x, posB.x);
    auto dy = avg(posA.y, posB.y);

    setPosition(dx, dy);
}

void Cell::interactBorder(double wallx, double wally, double wallwidth, double wallheight, 
                double move_amount, bool infinite)
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

    if (infinite)
    {
        _borderMove(left, right, top, bottom, wallwidth, wallheight);
    }
    else
    {
        _borderMove(left, right, top, bottom, move_amount, move_amount);
        reflected = _calcReflected(left, right, top, bottom, reflected);
        setRotation(reflected);
        moveVec(*this, move_amount);
    }
}

void Cell::_borderMove(bool left, bool right, bool top, bool bottom, double dx, double dy)
{
    // Corner cases -> Turn 180 degrees (Other methods, like facing the center of the board, resulted in bugs)
    if (left && top)
    {
        move(dx, dy);
    }
    else if (left && bottom)
    {
        move(dx, -dy);
    }
    else if (right && top)
    {
        move(-dx, dy);
    }
    else if (right && bottom)
    {
        move(-dx, -dy);
    }

    // Edge cases
    else
    {
        if (left)       
        {
            move(dx, 0);
        }
        else if (right) 
        {
            move(-dx, 0);
        }
        else if (top) 
        {
            move(0, dy);
        }
        else if (bottom) 
        {
            move(0, -dy);
        }
    }
}

double Cell::_calcReflected(bool left, bool right, bool top, bool bottom, const double& default_reflected)
{
    // If only one of right, top, left, and bottom is true (XOR) See http://stackoverflow.com/questions/1596668/logical-xor-operator-in-c
    double normal;
    if ((!right != !top) != (!left != !bottom))
    {
        if (left)       
        {
            normal = 0.;
        }
        else if (right) 
        {
            normal = 180.;
        }
        else if (top) 
        {
            normal = 90.;
        }
        else if (bottom) 
        {
            normal = 270.;
        }
        float inverse_normal = modAngle(normal + 180.);
        float incidence      = modAngle(rotation - inverse_normal);

        return modAngle(normal - incidence);
    }
    else
    {
        return default_reflected;
    }
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
                                             neighbor_center;
    /*
                           crowded           ? position{(visible_center.x + neighbor_center.x) / 2.0
                                                      , (visible_center.y + neighbor_center.y) / 2.0}
                                             : neighbor_center; 
                                            */

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
        affection += simulation_params.get("affection_prime");
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



