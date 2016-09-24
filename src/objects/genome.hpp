#pragma once
#include <SFML/Graphics.hpp>

namespace objects
{

class Genome
{
    const static double max_affection_prime;

public:
    Genome();
    Genome(Genome& a, Genome& b);

    sf::Color representation();

    double affectionPrime;
};

}

