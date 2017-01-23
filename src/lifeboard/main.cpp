/*
 * While openlifegui is licensed under the GPL, it requires SFML, a library with its own licensing.
 * As such, SFML is a prerequisite which must be obtained by the end user.
 */
 
#include "board.hpp"

int main()
{
    using objects::Board;
    Board board(2000, 2000);
    board.run(200, 2000, 2000); // Initial simulation with 50 starting cells

    return 0;
}
