#include "paramdict.hpp"

#include <sstream>
#include <iostream>
#include <iterator>
#include <assert.h>

#include <algorithm>

// As per http://stackoverflow.com/questions/2275135/splitting-a-string-by-whitespace-in-c
std::vector<std::string> split(std::string const &input) { 
    std::istringstream buffer(input);
    std::vector<std::string> ret{std::istream_iterator<std::string>(buffer), 
				 std::istream_iterator<std::string>()};
    return ret;
}

std::tuple<std::string, std::string> sepLine(const std::string& line)
{
    assert(not line.empty());
    auto terms = split(line);
    assert(terms.size() == 2); // KV pairs tend to come in pairs
    return std::make_tuple(terms[0], terms[1]);
}

ParamDict::ParamDict(std::string filename)
{
    print("Reading parameter dictionary from " + filename);
    for (auto line : tools::readFile(filename))
    {
        if (line.empty()){
            continue;
        }
        auto seperated = sepLine(line);
        auto key = std::get<0>(seperated);
        auto val_field = std::get<1>(seperated);
        print("\"" + key + "\", \"" + val_field + "\"");
        try
        {
            auto val = std::stod(val_field);
            print("Reading " + val_field + " as double setting");
            params[key] = val;
        }
        catch (const std::invalid_argument& e)
        {
            print("Reading " + val_field + " as string setting");
            assert(not std::any_of(val_field.begin(), val_field.end(), ::isdigit));
            // Make setting strings lowercase
            std::transform(val_field.begin(), val_field.end(), val_field.begin(), ::tolower);
            settings[key] = val_field;
        }
    }
}

ParamDict::ParamDict()
{}

double ParamDict::get(std::string param)
{
    auto got = params.find(param);
    if ( got == params.end() )
    {
        print(param + " is not an element of the paramdict");
        throw std::exception();
    }
    return params[param];
}

std::string ParamDict::getSetting(std::string s)
{
    auto got = settings.find(s);
    if ( got == settings.end() )
    {
        print(s + " is not an element of the settings dictionary");
        throw std::exception();
    }
    return settings[s];
}
