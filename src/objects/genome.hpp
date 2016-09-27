#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace objects
{

class Genome
{
public:
    Genome();
    Genome(Genome& a, Genome& b);

    sf::Color representation();

    // Retrieve a gene by name
    double gene(std::string);

private:
    std::unordered_map<std::string, double> genes;     // Dictionary string : double where double is a value between zero and one
    std::unordered_map<std::string, double> modifiers; // Dictionary string : double where double is a constant that gets multiplied by the gene

};

}

