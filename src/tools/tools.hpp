#include <random>
#include <vector>
#include <iostream>
#include <memory>
#include <sys/time.h> // To keep constant frametime
#include <unistd.h>
#include "physics.hpp"

namespace tools
{
std::mt19937 randomGenerator();
std::uniform_int_distribution<std::mt19937::result_type> dist(int a, int b);
std::uniform_real_distribution<double> dist(double a, double b);
int sign(std::mt19937);
unsigned long long getTime();
}
