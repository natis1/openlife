#include "entity.hpp"

namespace objects
{
Entity::Entity(double size, double life) : 
    size(size),
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
    // Got rid of check if already dead because pointless
    life -= n;
    
}

void Entity::regen(double n)
{
    // This function will only run if not dead.
    life += n;
    
}

double Entity::lifePercent()
{
    return life / total_life;
}

double Entity::getRadius()
{
    return this->radius;
}

position Entity::getPosition()
{
    return this->entityLocation;
}

void Entity::setPosition(double x, double y)
{
    this->entityLocation.x = x;
    this->entityLocation.y = y;
}

double Entity::getRotation()
{
    return this->rotation;
}

void Entity::setRotation(double theta)
{
    this->rotation = theta;
}

color Entity::getFillColor()
{
    return this->color;
}

void Entity::setFillColor(int r, int g, int b)
{
    this->color = {r, g, b};
}



}
