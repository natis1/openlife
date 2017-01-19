//Openlifecli is, like all of openlife, licensed under the GPLv3 or later.


#include "arena.hpp"

int main()
{
    using objects::Arena;
    Arena arena;
    arena.run(200, 2000, 2000); // Initial simulation with 50 starting cells

    return 0;
}
