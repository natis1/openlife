#pragma once
#include <unordered_map>
#include <string>
#include <tuple>
#include "tools.hpp"

//provides isDigit and digit checking
#include <locale>
#include <stdlib.h>

using tools::print;

class ParamDict
{
public:
    ParamDict(std::string filename);
    ParamDict();
    double get(std::string param);
    std::string getSetting(std::string setting);

private:
    std::unordered_map<std::string, double> params;
    std::unordered_map<std::string, std::string> settings;
};
