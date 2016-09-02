#include "entity.hpp"

Entity::Entity(double size, int life) : 
    sf::CircleShape(size),
    life(life),
    total_life(life)
{
}

bool Entity::alive()
{
    return life > 0;
}

void Entity::damage(int n)
{
    if (life > 0)
    {
        life -= n;
    }
}

void Entity::regen(int n)
{
    if (life < total_life)
    {
        life += n;
    }
}

double Entity::lifePercent()
{
    return (double)life / (double)total_life;
}

void Entity::update()
{
}
