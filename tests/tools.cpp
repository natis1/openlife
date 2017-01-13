#include "catch.hpp"
#include "../src/libopenlife/tools.hpp"

TEST_CASE( "tools package works" )
{
    const auto assertEqual = [](auto a, auto b)
    {
        REQUIRE((a == b));
    };

    /*
    SECTION ("standard matching works")
    {
        assertEqual(hello(expected).consumed , expected);
    }
    */
}

