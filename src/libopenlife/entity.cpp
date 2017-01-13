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

void Entity::move(double dx, double dy)
{
    this->entityLocation.x += dx;
    this->entityLocation.y += dy;
}


double Entity::getRadius()
{
    return this->radius;
}

void Entity::setRadius(double radius)
{
    this->radius = radius;
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
    return this->entityColor;
}

int Entity::getApproximateFillColor()
{
    unsigned char colors[4];
    
    colors[0] = (char) this->entityColor.r;
    colors[1] = (char) this->entityColor.g;
    colors[2] = (char) this->entityColor.b;
    colors[3] = (char) this->entityColor.a;
    
    // Big|little endian independant, thankfully.
    return (colors[0] << 24) | (colors[1] << 16) | (colors[2] << 8) | colors[3];
}


void Entity::setFillColor(double r, double g, double b, double a)
{
    this->entityColor = {r, g, b, a};
}

void Entity::setFillColor(color c)
{
    this->entityColor = c;
}



}
