
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/point.h"

point_t *init_point(size_t dim, double *components) {
    point_t *point = (point_t *)calloc(1, sizeof(point_t));
    if (!point)
    {
        perror("init_point");
        return NULL;
    }

    point->dim = dim;
    point->components = components;

    return point;
}

int pntcmp(point_t *p1, point_t *p2)
{
    return distance(p1, p2);
}

double distance(point_t *p1, point_t *p2)
{
    double s = 0;
    for (size_t i = 0; i < p1->dim; ++i)
    {
        double d = p2->components[i] - p1->components[i];
        s += d*d;
    }

    return sqrt(s);
}

