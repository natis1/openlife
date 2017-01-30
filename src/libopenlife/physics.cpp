#include "physics.hpp"
#include <assert.h>

double modAngle(double theta)
{
    theta = remainder(theta, 360.);
    while (theta < 0.)
    {
        theta += 360.;
    }
    return theta;
}
