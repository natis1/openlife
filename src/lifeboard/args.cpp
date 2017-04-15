#include "args.hpp"


struct options parseArgs(int argc, char ** args)
{
    std::vector<std::string> arguements;
    arguements.reserve(argc);
    for (int i = 1; i < argc; i++){
        arguements.push_back(args[i]);
    }
    return extractArgs(arguements);
}


struct options extractArgs(std::vector<std::string> args)
{
    int length;
    args.size() > 6 ? length = 6: length = args.size();
    for (int i = length; i < 6; i++) {
        args.push_back("100");
    }
    return options {args[0], stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), stoi(args[5])};
        
    
}

