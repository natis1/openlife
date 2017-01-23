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

private:
    std::unordered_map<std::string, double> params;

};
