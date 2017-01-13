#include "catch.hpp"
#include "../src/libopenlife/tools.hpp"

TEST_CASE( "tools package works" )
{
    SECTION("Angle and distance formulas work")
    {
        Cell a, b;
        REQUIRE(distance(a, b) == 0);
        a.move(1, 0); // Move right 1
        REQUIRE(distance(a, b) == 1);
        REQUIRE(angle(a.getPosition(), b.getPosition()) == 0);
        a.move(0, 1); // Move up 1
        REQUIRE(angle(a.getPosition(), b.getPosition()) == 45);
    }
}

