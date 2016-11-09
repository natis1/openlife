#include "catch.hpp"
#include "../src/tools/tools.hpp"

TEST_CASE( "tools package works" )
{
    const auto assertEqual = [](auto a, auto b)
    {
        REQUIRE((a == b));
    };

    SECTION("")
    {

    }

    /*
    template <typename T>
    T degrees(T t)
    {
        return t * 57.29577951308232;
    }

    template <typename T>
    T radians(T t)
    {
        return t * 0.017453292519943;
    }

    template <typename T1, typename T2>
    double distance(T1& t1, T2& t2)
    {
        auto a = t1.getPosition();
        auto b = t2.getPosition();
        double dx = abs(a.x - b.x);
        double dy = abs(a.y - b.y);
        return hypot(dx, dy);
    }

    // Compute angle between two objects in degrees
    template <typename T1, typename T2>
    double angle(T1& t1, T2& t2)
    { 
        auto a = t1.getPosition();
        auto b = t2.getPosition();
        double angle = atan2(a.y - b.y, a.x - b.x);
        angle = degrees(angle);

        
        if (angle < 0) {
            angle = 360. + angle;
        }
        
        if (angle > 360.) {
            angle = (int)angle % 360;
        }
        return angle;
    }

    // Move on a vector with magnitude amount along current rotation
    template <typename T>
    void moveVec(T& t, double amount)
    {
        t.move( amount * cos(radians(t.getRotation())) , amount * sin(radians(t.getRotation())));
    }
    */
}

