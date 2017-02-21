//Openlifecli is, like all of openlife, licensed under the GPLv3 or later.


#include "arena.hpp"

int main(int argc, char** args)
{
    using objects::Arena;
    Arena arena;
    arena.run(100, 500, 500); // Initial simulation with 50 starting cells

    return 0;
}
