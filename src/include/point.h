
#ifndef KMEANS_POINT
#define KMEANS_POINT

#include <stdlib.h>

typedef struct {
    size_t dim;
    double *components;
} point_t;

point_t *init_point(size_t dim, double *components);

int pntcmp(point_t *p1, point_t *p2);
double distance(point_t *p1, point_t *p2);
 
#endif

