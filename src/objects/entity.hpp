#pragma once
#include <SFML/Graphics.hpp>

class Entity : public sf::CircleShape
{

public:
    Entity(double size, double life);
    virtual void update();
    bool alive();
    void damage(double n=1.);
    void regen(double n=1.);
    double lifePercent();

private:
    double life;
    double total_life;
};

