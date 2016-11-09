#include "objects/board.hpp"

int main()
{
    using objects::Board;
    Board board;
    board.run(2, 2000, 2000); // Initial simulation with 50 starting cells

    return 0;
}
