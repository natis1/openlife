#include "genome.hpp"
#include "../tools/tools.hpp"

namespace objects
{
    const double Genome::max_affection_prime = 2.77;

    Genome::Genome() 
    {
        auto affectionPrimeDist = tools::dist(0., Genome::max_affection_prime);
        auto generator = tools::randomGenerator();

        affectionPrime = affectionPrimeDist(generator);
    }

    Genome::Genome(Genome& a, Genome& b) : 
        affectionPrime(tools::avg(a.affectionPrime, b.affectionPrime))
    {
    }

    sf::Color Genome::representation()
    {
        float red  = 255.0f * (affectionPrime / Genome::max_affection_prime);

        return sf::Color(red, 16.0f, 128.0f);
    }
}
