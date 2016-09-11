#include "objects/board.hpp"

int main()
{
    using objects::Board;
    Board board;
    board.run(80); // Initial simulation with 80 starting cells

    return 0;
}
