#include <stdlib.h>
#include <math.h>
#include "utility.h"

const double infinity = INFINITY;
const double pi = 3.1415926535897932385;


double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

double random_double_unit()
{
    // Returns a random real in [0,1).
    return (double)rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max)
{
    // Returns a random real in [min,max).
    return min + (max-min)*random_double_unit();
}

double clamp(double x, double min, double max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}