/*
 * While openlifegui is licensed under the GPL, it requires SFML, a library with its own licensing.
 * As such, SFML is a prerequisite which must be obtained by the end user.
 */
 
#include "board.hpp"
#include "args.hpp"

int main(int argc, char** args)
{
    using objects::Board;
    
    
    
    
    Board board(1920, 1080);
    board.run(100, 1920, 1080); // Initial simulation with 50 starting cells
    
    
    return 0;
}
