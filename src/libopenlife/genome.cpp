#include "genome.hpp"


namespace objects
{
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

    color Genome::representation()
    {
        using tools::avg;

        std::vector<double> components;
        for (auto& kv : genes)
        {
            components.push_back(kv.second / modifiers[kv.first]); // Retrieve unmodified gene values (between zero and one)
        }

        // Initial color gets averaged with gene values * 255
        double red   = 128.;
        double green = 128.;
        double blue  = 128.;

        // Make color represent the average of all gene components
        int counter = 0;
        for (double component : components)
        {
            if (counter == 0)
                red = avg(255. * component, red);
            else if (counter == 1)
                green = avg(255. * component, green);
            else if (counter == 2)
            {
                blue = avg(255. * component, blue);
                counter = 0; // Cycle back to red
            }

            counter++;
        }

        return {red, green, blue, 255.};
    }

    double Genome::gene(std::string name)
    {
        return genes[name];
    }
}
