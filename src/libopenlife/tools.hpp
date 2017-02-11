#pragma once
#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <sys/time.h> // To keep constant frametime
#include <unistd.h>
#include <math.h>
#include <algorithm>

#include "physics.hpp"

namespace tools
{

// Normally defined types
std::mt19937 randomGenerator();
std::uniform_int_distribution<std::mt19937::result_type> dist(int a, int b);
std::uniform_real_distribution<double> dist(double a, double b);
int sign(std::mt19937);
unsigned long long getTime();
void writeFile(std::vector<std::string> content, std::string filename);
std::vector<std::string> readFile(std::string filename);

template <typename T>
T avg (T a, T b)
{
    return ((a + b) / 2.);
}

template<typename T>
void print(T t) {
    std::cout << t << "\n";
}

template<typename T, typename... Args>
void print(T first, Args... args) {
    print(first);
    print(args...);
    std::cout << std::endl;
}

template <typename T>
void writeCSV(std::string filename, std::vector<T> items)
{
    std::ofstream csvfile;
    csvfile.open(filename);
    for (const auto& item : items)
    {
        std::string line = item->csv() + " " ;
        csvfile << line + "\n";
    }
    csvfile.close();
}

double getLimitedAngle(double ideal, double current, double limit);
double angleDiff(double a, double b);

}
