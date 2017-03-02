#include <string>
#include <vector>
#include <stdio.h>

struct options {
    std::string parameterFile;
    int nCells;
    int width;
    int height;
    int spawnWidth;
    int spawnHeight;
};



struct options extractArgs(std::vector<std::string> args);
struct options parseArgs(int argc, char** args);

