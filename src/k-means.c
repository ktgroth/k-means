
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

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

void group_add(group_t *group, point_t *point)
{
    group->points = (point_t **)realloc(group->points, (group->size + 1) * sizeof(point_t *));
    group->points[group->size++] = point;
}

point_t *get_next_midpoint(group_t *group)
{
    double dim = group->points[0]->dim;
    double *mc = (double *)calloc(dim, sizeof(double));
    for (size_t i = 0; i < group->size; ++i)
    {
        for (size_t j = 0; j < dim; ++j)
            mc[j] += group->points[i]->components[j];
    }

    if (!group->size)
        exit(1);

    for (size_t i = 0; i < dim; ++i)
        mc[i] /= group->size;

    return init_point(dim, mc);
}

void sanatize(group_t *group, point_t *mp)
{
    free(group->points);

    group->points = (point_t **)calloc(1, sizeof(point_t *));
    group->size = 1;
    group->points[0] = mp;
}

int in(size_t *arr, size_t v, size_t k)
{
    for (size_t i = 0; i < k && arr[i] != (size_t)-1; ++i)
        if (arr[i] == v)
            return 1;

    return 0;
}

group_t *cluster(size_t k, size_t n, point_t **points)
{
    srand(time(NULL));
    group_t *groups = init_group(k);

    size_t *starts = (size_t *)calloc(k, sizeof(size_t));
    point_t **centroids = (point_t **)calloc(k, sizeof(point_t *));
    for (size_t i = 0; i < k; ++i)
    {
        starts[i] = -1;
        size_t r = rand() % n;
        while (in(starts, r, k))
            r = rand() % n;

        double *components = (double *)calloc(points[r]->dim, sizeof(double));
        point_t *point = init_point(points[r]->dim, memcpy(components, points[r]->components, points[r]->dim * sizeof(double)));
        group_add(&groups[i], point);
        centroids[i] = point;
    }
    free(starts);

    double epsilon = 1e-15;
    while (true)
    {
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

        point_t **next_mps = (point_t **)calloc(k, sizeof(point_t *));
        next_mps[0] = get_next_midpoint(&groups[0]);
        double max_d = distance(centroids[0], next_mps[0]);
        for (size_t j = 1; j < k; ++j)
        {
            next_mps[j] = get_next_midpoint(&groups[j]);
            double d = distance(centroids[j], next_mps[j]);
            if (d > max_d)
                max_d = d;
        }
        printf("%lg\r", max_d);
        fflush(stdout);
        usleep(100000);

        if (max_d < epsilon)
        {
            free(next_mps);
            return groups;
        }
        for (size_t j = 0; j < k; ++j)
        {
            sanatize(&groups[j], next_mps[j]);
            centroids[j] = groups[j].points[0];
        }
        free(next_mps);
    }
}

