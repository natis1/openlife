#include <random>
#include <vector>
#include <iostream>
#include <memory>
#include "physics.hpp"

std::mt19937 randomGenerator();
std::uniform_int_distribution<std::mt19937::result_type> dist(int a, int b);
int sign(std::mt19937);
