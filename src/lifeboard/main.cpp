/*
 * While openlifegui is licensed under the GPL, it requires SFML, a library with its own licensing.
 * As such, SFML is a prerequisite which must be obtained by the end user.
 */
 
#include "board.hpp"
#include "args.hpp"

int main(int argc, char** args)
{
    using objects::Board;
    
    if (argc == 7) {
        
        options opts = parseArgs(argc, args);
        Board board(opts.spawnWidth, opts.spawnHeight);
        board.run(opts.nCells, opts.width, opts.height, opts.parameterFile);
        
        
    } else if (argc == 5) {
        
        options opts = parseArgs(argc, args);
        Board board(1920, 1080);
        board.run(opts.nCells, opts.width, opts.height, opts.parameterFile);
        
        
    } else if (argc == 3){
        options opts = parseArgs(argc, args);
        Board board(1920, 1080);
        board.run(opts.nCells, 1920, 1080, opts.parameterFile);
        
    } else if (argc >= 2){
        options opts = parseArgs(argc, args);
        Board board(1920, 1080);
        board.run(100, 1920, 1080, opts.parameterFile);
        
    } else {
        Board board(1920, 1080);
        board.run(100, 1920, 1080, "params.txt"); // Initial simulation with 50 starting cells
    }
    
    return 0;
}
