#include "catch.hpp"
#include "../src/libopenlife/tools.hpp"
#include "../src/libopenlife/cell.hpp"

using tools::print;

TEST_CASE( "tools package works" )
{
    SECTION("Angle and distance formulas work")
    {
        const auto almostEqual = [](auto a, auto b)
        { 
            bool equal = abs(a - b) < 0.0000001; 
            if (not equal)
            {
                print(std::to_string(a) + " is not equal to " + std::to_string(b));
            }
            return equal;
        };

        Cell a, b;
        REQUIRE(almostEqual(distance(a, b), 0.));
        a.move(1, 0); // Move right 1
        REQUIRE(almostEqual(distance(a, b), 1.) );
        REQUIRE(almostEqual(angle(a.getPosition(), b.getPosition()), 0.) );
        a.move(0, 1); // Move up 1
        REQUIRE(almostEqual(angle(a.getPosition(), b.getPosition()), 45.) );
        a.move(-2, -2.); // Move to fourth quadrant 
        REQUIRE(almostEqual(angle(a.getPosition(), b.getPosition()), 225.) );
    }
}

