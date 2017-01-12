#include "arena.hpp"

int main()
{
    using objects::Arena;
    Arena arena;
    arena.run(50, 2000, 2000); // Initial simulation with 50 starting cells

    return 0;
}
