#include <unordered_map>
#include <string>
#include <tuple>
#include "tools.hpp"

using tools::print;

class ParamDict
{
public:
    ParamDict(std::string filename);
    double get(std::string param);

private:
    std::unordered_map<std::string, double> params;

};
