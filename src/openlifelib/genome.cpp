#include "genome.hpp"
#include "../tools/tools.hpp"

// Create initial genes with random number generators
Genome::Genome() 
{
    // Describe the initial genome
    modifiers = {
        {"affection_prime", 2.77}, // "Genome has a value affection_prime, which is between zero and 2.77"
    };

    auto randDist  = tools::dist(0., 1.);
    auto generator = tools::randomGenerator();

    for (auto& kv : modifiers)
    {
        genes[kv.first] = kv.second * randDist(generator); 
    }
}

// Crossover algorithm is, for now, the average genes of two genomes
Genome::Genome(Genome& a, Genome& b)  
{
    std::vector<std::string> gene_names;
    for (auto& kv : genes)
    {
        gene_names.push_back(kv.first);
    }
    // Seperated this into two loops so that the dictionary isn't modified as it is iterated over
    for (auto& gene_name : gene_names)
    {
        genes[gene_name] = tools::avg(a.gene(gene_name), b.gene(gene_name));
    }
}

sf::Color Genome::representation()
{
    return sf::Color(195.f, 127.f, 255.f);
    // If there are ever more genes, this representation function will make more sense
    /*
    using tools::avg;

    std::vector<double> components;
    for (auto& kv : genes)
    {
        components.push_back(kv.second / modifiers[kv.first]); // Retrieve unmodified gene values (between zero and one)
    }

    // Initial color gets averaged with gene values * 255
    double red   = 0.;
    double green = 0.;
    double blue  = 0.;

    // Lambda for factoring in intensities for a particular color (code re-use)
    const auto factorComponent= [](double& color, const double& intensity)
    {
        // Either average the existing color with an intensity, or set the color to its initial value
        color = color > 1. ? avg(intensity, color) : intensity;
    };

    // Make color represent the average of all gene components
    int counter = 0;
    for (double component : components)
    {
        double intensity = 255. * component;
        if (counter == 0)
        {
            factorComponent(red,   intensity);
        }
        else if (counter == 1)
        {
            factorComponent(green, intensity);
        }
        else if (counter == 2)
        {
            factorComponent(blue,  intensity);
            counter = -1; // Cycle back to red
        }
        counter++;
    }

    return sf::Color(red, green, blue);
    */
}

double Genome::gene(std::string name)
{
    return genes[name];
}
