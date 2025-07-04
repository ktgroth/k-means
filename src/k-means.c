
#include <stdio.h>
#include <time.h>

#include "include/point.h"
#include "include/k-means.h"

group_t *init_group(size_t k)
{
    group_t *groups = (group_t *)calloc(k, sizeof(group_t));
    if (!groups)
    {
        perror("Allocating Groups ");
        exit(1);
    }

    for (size_t i = 0; i < k; ++i)
    {
        groups[i].size = 0;
        groups[i].points = NULL;
    }

    return groups;
}

void group_add(group_t *group, point_t point)
{
    group->points = (point_t *)realloc(group->points, (group->size + 1) * sizeof(point_t));
    group->points[group->size++] = point;
}

void set_next_midpoint(group_t *group)
{
    double mx = 0, my = 0;
    for (size_t i = 0; i < group->size; ++i)
    {
        mx += creal(group->points[i]);
        my += cimag(group->points[i]);
    }

    if (!group->size)
        return;

    mx /= group->size;
    my /= group->size;

    free(group->points);
    group->points = (point_t *)calloc(1, sizeof(point_t));

    group->size = 1;
    group->points[0] = CMPLX(mx, my);
}

int in(size_t *arr, size_t v, size_t k)
{
    for (size_t i = 0; i < k && arr[i] != (size_t)-1; ++i)
        if (arr[i] == v)
            return 1;

    return 0;
}

group_t *cluster(size_t k, size_t n, point_t *points)
{
    srand(time(NULL));
    group_t *groups = init_group(k);

    size_t *starts = (size_t *)calloc(k, sizeof(size_t));
    for (size_t i = 0; i < k; ++i)
    {
        starts[i] = -1;
        size_t r = rand() % n;
        while (in(starts, r, k))
            r = rand() % n;

        group_add(&groups[i], points[r]);
    }
    free(starts);

    size_t iterations = 10000;
    for (size_t i = 0; i < iterations; ++i)
    {
        for (size_t j = 0; j < k; ++j)
            set_next_midpoint(&groups[j]);

        for (size_t j = 0; j < n; ++j)
        {
            group_t *min_group = &groups[0];
            double min_distance = distance(groups[0].points[0], points[j]);
            for (size_t l = 1; l < k; ++l)
            {
                double d = distance(groups[l].points[0], points[j]);
                if (d < min_distance)
                {
                    min_group = &groups[l];
                    min_distance = d;
                }
            }
            group_add(min_group, points[j]);
        }
    }

    return groups;
}

