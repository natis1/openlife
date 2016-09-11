#include "tools.hpp"

namespace tools
{
std::mt19937 randomGenerator()
{
    // Create and seed a random number generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    return rng;
}

std::uniform_int_distribution<std::mt19937::result_type> dist(int a, int b)
{
    return std::uniform_int_distribution<std::mt19937::result_type>(a, b); 
} 

std::uniform_real_distribution<double> dist(double a, double b)
{
    return std::uniform_real_distribution<double> (a, b);
}

int sign(std::mt19937 generator)
{
    if (dist(0, 10000)(generator) < 5000)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

unsigned long long getTime()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return now.tv_usec + (unsigned long long)now.tv_sec * 1000000;
}

}
