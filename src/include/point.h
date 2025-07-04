
#ifndef KMEANS_POINT
#define KMEANS_POINT

#include <complex.h>

typedef double complex point_t;

int pntcmp(point_t *p1, point_t *p2);
double distance(point_t *p1, point_t *p2);
 
#endif

