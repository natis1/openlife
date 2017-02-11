#include "catch.hpp"
#include "../src/libopenlife/tools.hpp"
#include "../src/libopenlife/cell.hpp"

using tools::print;
const auto almostEqual = [](auto a, auto b)
{ 
    bool equal = abs(a - b) < 0.0000001; 
    if (not equal)
    {
        print(std::to_string(a) + " is not equal to " + std::to_string(b));
    }
    return equal;
};

const auto requireEq = [](auto a, auto b)
{
    REQUIRE( almostEqual(a, b));
};

TEST_CASE( "tools package" )
{
    SECTION("Angle and distance formulas")
    {

        Cell a(10, 10);
        Cell b(10, 10);
        requireEq(distance(a, b), 0.);
        a.move(1, 0); // Move right 1
        requireEq(distance(a, b), 1.);
        requireEq(angle(b.getPosition(), a.getPosition()), 0.);
        a.move(0, 1); // Move up 1
        requireEq(angle(b.getPosition(), a.getPosition()), 45.);
        a.move(-2, -2.); // Move to fourth quadrant 
        requireEq(angle(b.getPosition(), a.getPosition()), 225.);
    }

    SECTION("Turn rate limiting code")
    {
        using tools::getLimitedAngle;
        requireEq(getLimitedAngle(0., 10., 5.) ,  5.);
        requireEq(getLimitedAngle(350., 0., 5.), -5.);
        requireEq(getLimitedAngle(73., 00., 360.1) ,  73.);
    }
}

