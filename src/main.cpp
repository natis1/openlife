//Openlifecli is, like all of openlife, licensed under the GPLv3 or later.
#include "arena.hpp"
#include "cliargparser.hpp"

int main(int argc, char** args)
{
    using objects::Arena;
    
    
    
    if (argc >= 5) {
        
        options opts = parseArgs(argc, args);
        Arena arena;
        arena.run(opts.nCells, opts.width, opts.height, opts.parameterFile);
        
        
    } else if (argc == 3){
        options opts = parseArgs(argc, args);
        Arena arena;
        arena.run(opts.nCells, 1920, 1080, opts.parameterFile);
        
    } else if (argc >= 2){
        options opts = parseArgs(argc, args);
        Arena arena;
        arena.run(100, 1920, 1080, opts.parameterFile);
        
    } else {
        Arena arena;
        arena.run(100, 1920, 1080, "params.txt"); // Initial simulation with 50 starting cells
    }
    

    return 0;
}
