
#ifndef KMEANS
#define KMEANS

#include <stdlib.h>

#include "point.h"

typedef struct group
{
  size_t size;
  point_t *points;
} group_t;

group_t *init_group(size_t k);
void group_add(group_t *group, point_t point);

group_t *cluster(size_t k, size_t n, point_t *points);

#endif

