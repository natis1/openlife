#include "entity.hpp"

Entity::Entity(double size, double life) : 
    sf::CircleShape(size),
    life(life),
    total_life(life)
{
    setOrigin(sf::Vector2f(size, size));
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
