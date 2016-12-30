#include "catch.hpp"
#include "../src/tools/tools.hpp"
#include "../src/openlifelib/cell.hpp"

using namespace tools;

TEST_CASE( "tools package works" )
{
    SECTION("Angle and distance formulas work")
    {
        Cell a, b;
        REQUIRE(distance(a, b) == 0);
        a.move(1, 0); // Move right 1
        REQUIRE(distance(a, b) == 1);
        REQUIRE(angle(a, b) == 0);
        a.move(0, 1); // Move up 1
        REQUIRE(angle(a, b) == 45);
    }
}

