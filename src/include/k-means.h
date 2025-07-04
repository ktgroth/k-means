
#ifndef KMEANS
#define KMEANS

#include <stdlib.h>

#include "point.h"

typedef struct group
{
  size_t size;
  point_t *points;
} group_t;

group_t *k_means(size_t k, size_t n, point_t *points);

#endif

