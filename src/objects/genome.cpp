#include "genome.hpp"
#include "../tools/tools.hpp"

namespace objects
{
    const double Genome::max_affection_prime = 2.77;

    Genome::Genome() 
    {
        auto affectionPrimeDist = tools::dist(0., Genome::max_affection_prime);
        auto generator = tools::randomGenerator();

        attributes["affection_prime"] = affectionPrimeDist(generator);
    }

    Genome::Genome(Genome& a, Genome& b)  
    {
        attributes["affection_prime"] = tools::avg(a.gene("affection_prime"), b.gene("affection_prime"));
    }

    sf::Color Genome::representation()
    {
        float red  = 255.0f * (gene("affection_prime") / Genome::max_affection_prime);

        return sf::Color(red, 16.0f, 128.0f);
    }

    double Genome::gene(std::string name)
    {
        return attributes[name];
    }
}
