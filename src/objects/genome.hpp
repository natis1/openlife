#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace objects
{

class Genome
{
    const static double max_affection_prime;

public:
    Genome();
    Genome(Genome& a, Genome& b);

    sf::Color representation();

    double gene(std::string);

private:
    std::unordered_map<std::string, double> attributes;

};

}

