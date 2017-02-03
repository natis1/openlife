/*
 * While openlifegui is licensed under the GPL, it requires SFML, a library with its own licensing.
 * As such, SFML is a prerequisite which must be obtained by the end user.
 */
 
#include "board.hpp"

int main()
{
    using objects::Board;
    Board board(1000, 1000);
    board.run(100, 1000, 1000); // Initial simulation with 50 starting cells

    return 0;
}
