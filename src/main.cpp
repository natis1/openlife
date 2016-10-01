#include "objects/board.hpp"

int main()
{
    using objects::Board;
    Board board;
    board.run(80, 2000, 2000); // Initial simulation with 80 starting cells

    return 0;
}
