#include "openlifelib/board.hpp"

int main()
{
    Board board;
    board.run(50, 2000, 2000, false, false); // Initial simulation with 50 starting cells, with debug information hidden

    return 0;
}
