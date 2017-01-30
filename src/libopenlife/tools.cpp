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

std::vector<std::string> readFile(std::string filename)
{
    // Read an entire file, line by line into a vector
    std::vector<std::string> content;

    std::ifstream infile(filename);
    std::string line;

    while (std::getline(infile, line))
    {
        content.push_back(line);
    }

    return content;
}

void writeFile(std::vector<std::string> content, std::string filename)
{
    // Write a vector of lines into a file
    std::ofstream file;
    file.open(filename);
    for (auto line : content)
    {
        file << (line + "\n");
    }
    file.close();
}

double getLimitedAngle(double ideal, double current, double limit)
{
    auto opposite = modAngle(current + 180.);
    auto diff = angleDiff(ideal, current);
    if (diff < limit)
    {
        return ideal;
    }
    else if (ideal > current and ideal < opposite)
    {
        return current + limit;
    }
    else
    {
        return current - limit;
    }
}

double angleDiff(double a, double b)
{
    return std::min( abs(a - b), abs(a - b - 360.) );
}

}
