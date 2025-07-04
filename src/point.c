
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/point.h"

int pntcmp(point_t p1, point_t p2)
{
    return distance(p1, p2) == 0;
}

double distance(point_t p1, point_t p2)
{
    double dx = creal(p2) - creal(p1);
    double dy = cimag(p2) - cimag(p1);

    return sqrt(dx*dx + dy*dy);
}

