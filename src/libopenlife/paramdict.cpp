#include "paramdict.hpp"

std::tuple<std::string, double> sepParamLine(std::string line)
{
    bool foundString = false;
    bool foundDouble = false;
    // is the iterator currently inside a variable?
    bool isLookingAtData = false;
    
    auto startkey = line.begin();
    auto endkey   = line.begin(); 
    auto startval = line.begin();
    auto endval =  line.begin();
    
    //An idiot proof error detector will always attract a larger idiot.
    for (auto it = line.begin(); it != line.end(); it++){
        if (!foundString) {
            if (*it == ' ' || *it == '\t'){
                continue;
            } else if (*it == EOF) {
                std::cerr << "MAJOR ERROR: INVALID DATA IN LINE:" << std::endl << line << std::endl << "Unexpected EOF or 0x04" << std::endl;
                return std::make_tuple("INVALID_TUPLE", 0.0);
            } else {
                startkey = it;
                //Protects against keys with a length of 1
                endkey   = it;
                foundString = true;
                isLookingAtData = true;
            } 
            
            // foundString is implicit in this else if
        } else if (isLookingAtData && !foundDouble) {
            if (*it != ' ' &&  *it != '\t' && *it != EOF) {
                continue;
            } else if (*it == ' ' || *it == '\t') {
                endkey = it;
                isLookingAtData = false;
                continue;
            } else {
                std::cerr << "MAJOR ERROR: INVALID DATA IN LINE:" << std::endl << line << std::endl << "Unexpected EOF or 0x04" << std::endl;
                return std::make_tuple("INVALID_TUPLE", 0.0);
            }
            
            // !islookingatdata and foundString are implicit in this else if
        } else if (!foundDouble) {
            if (*it == ' ' || *it == '\t') {
                continue;
            } else if (isdigit(*it) || *it == '.' || *it == '-') {
                startval = it;
                endval   = it;
                isLookingAtData = true;
                foundDouble = true;
            } else if (*it != EOF) {
                std::cerr << "Minor error: Unable to identify the value in line:" << std::endl << line << std::endl << "This line will be ignored. Please fix it." << std::endl;
                return std::make_tuple("INVALID_TUPLE", 0.0);
            } else {
                std::cerr << "MAJOR ERROR: INVALID DATA IN LINE:" << std::endl << line << std::endl << "Unexpected EOF or 0x04" << std::endl;
                return std::make_tuple("INVALID_TUPLE", 0.0);
            }
        } else {
            if (*it == ' ' || *it == '\t') {
                break;
            } else if (isdigit(*it) || *it == '.') {
                endval = it;
                continue;
            } else if (*it != EOF) {
                std::cerr << "Minor error: Unable to identify the value in line:" << std::endl << line << std::endl << "This line will be ignored. Please fix it." << std::endl;
                return std::make_tuple("INVALID_TUPLE", 0.0);
            } else {
                std::cerr << "MAJOR ERROR: INVALID DATA IN LINE:" << std::endl << line << std::endl << "Unexpected EOF or 0x04" << std::endl;
                return std::make_tuple("invalid_tuple", 0.0);
            }
            
        }
    }
    
    if (line.length() == 0) {
        std::cerr << "Found a blank line?! This should NEVER happen!!" << std::endl << "If you haven't modified the code, then please alert us at https://github.com/natis1/openlife/issues" << std::endl;
        return std::make_tuple("invalid_tuple", 0.0);
    }
    
    // Check if user made a valid double
    
    char* validDouble;
    std::string val(startval, endval + 1);
    double convertedDouble = strtod(val.c_str(), &validDouble);

    //Will be true if the operation fails
    if (*validDouble) {
        std::cerr << "Minor error: Unable to identify the value in line:" << std::endl << line << std::endl << "This line will be ignored. Please fix it." << std::endl;
        return std::make_tuple("invalid_tuple", 0.0);
    }
    else {
        std::string key(startkey, endkey);
        return std::make_tuple(key, convertedDouble);
    }
    
    
}

ParamDict::ParamDict(std::string filename)
{
    print("Reading parameter dictionary from " + filename);
    for (auto line : tools::readFile(filename))
    {
        if (line.length() == 0){
            continue;
        }
        auto seperated = sepParamLine(line);
        auto key = std::get<0>(seperated);
        if (key == "INVALID_TUPLE") {
            continue;
        }
        auto val = std::get<1>(seperated);
        params[key] = val;
        print("\"" + key + "\", \"" + std::to_string(val) + "\"");
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
