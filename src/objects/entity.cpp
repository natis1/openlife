#include "entity.hpp"

namespace objects
{
Entity::Entity(double size, double life) : 
    sf::CircleShape(size),
    life(life),
    total_life(life)
{
}

bool Entity::alive()
{
    return life > 0;
}

void Entity::damage(double n)
{
    if (life > 0)
    {
        life -= n;
    }
}

void Entity::regen(double n)
{
    if (life < total_life)
    {
        life += n;
    }
}

double Entity::lifePercent()
{
    return life / total_life;
}

void Entity::update()
{
}
}
