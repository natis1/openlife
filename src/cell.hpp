#pragma once
#include <SFML/Graphics.hpp>
#include "physics.hpp"

class Cell : public sf::CircleShape
{
public:
    Cell();
    void update();
    void bounce();

private:
    int life;
};

