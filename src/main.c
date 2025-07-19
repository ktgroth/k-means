
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "include/point.h"
#include "include/k-means.h"

#define RANGE 0.0
#define SPREAD 10.0


typedef struct
{
    point_t **points;
    size_t size;
} point_array_t;

point_array_t read_points(char *ifilename)
{
    FILE *ifile = fopen(ifilename, "r");
    if (!ifile)
    {
        perror("Could Not Open File");
        exit(1);
    }

    char *line = NULL;
    size_t n = 0;
    ssize_t read;

    size_t d = 0;
    if (fscanf(ifile, "%ld\n", &d) > 1)
    {
        perror("fscanf(ifile, \"%d\\n\", &d)"); 
        exit(1);
    }

    size_t i = 0;
    point_t **points = NULL;
    while ((read = getline(&line, &n, ifile)) != -1)
    {
        double *components = (double *)calloc(d, sizeof(double));
        for (size_t j = 0; j < d; ++j)
        {
            if (fscanf(ifile, "%lf", &components[j]) > 1)
            {
                perror("fscanf(ifile, \"%lf\", &components[j])");
                exit(1);
            }
        }

        points = (point_t **)realloc(points, (i + 1) * sizeof(point_t *));
        points[i++] = init_point(d, components);
    }

    free(line);
    fclose(ifile);

    point_array_t ret = { .points=points, .size=i };
    return ret;
}

inline double rand_double(double min, double max)
{
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

point_t **generate_points(size_t k, size_t n, size_t dim)
{
    srand(time(NULL));

    size_t t = 0;
    point_t **points = NULL;
    for (size_t i = 0; i < k; ++i) {
        double *group_base = (double *)calloc(dim, sizeof(double));
        printf("(\t");
        for (size_t j = 0; j < dim; ++j)
        {
            group_base[j] = rand_double(-RANGE, RANGE);
            printf("\%-3.6lf\t", group_base[j]);
        }
        printf(")\n");

        for (size_t j = 0; t < n && j < (size_t)ceil((double)n / k); ++j, ++t)
        {
            double *components = (double *)calloc(dim, sizeof(double));
            for (size_t l = 0; l < dim; ++l)
                components[l] = group_base[l] + rand_double(-SPREAD, SPREAD);

            points = (point_t **)realloc(points, (t + 1) * sizeof(point_t *));
            points[t] = init_point(dim, components);
        }

        free(group_base);
    }

    return points;
}

void print_help()
{
    printf("Usage: .k-means [OPTION]... [FILE]\n"
           "Groups clusters of points together based on the k-means algorithm.\n\n"
           "Generic Program Information:\n"
           "\t--help: Output a usage message and exit.\n"
           "\t-v, --version: Output the version number of k-means and exit.\n"
           "IO Control:\n"
           "\t-i, --input=FILE: Input file containing the points to be grouped.\n"
           "\t-o, --output=FILE: Output file to list the groups and their constituent points.\n"
           "Point Generation Control:\n"
           "\t-n, --genn=N: Generate N points which will be grouped using k-means.\n"
           "\t-d, --dim=D: Points will have dimension D.\n"
           "\t-k, --groups=K: K groups of points.\n\n");
}

int main(int argc, char *argv[])
{
    char *ifilename = NULL, *ofilename = NULL;
    size_t k = 0, n = 0, d = 0;

    if (argc < 2)
    {
        print_help();
        return 1; 
    } else {
    if (!strcmp(argv[1], "--help"))
    {
        print_help();
        return 0;
    }

    else if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
    {
        printf("Written by Kasen Groth; see\n"
        "<https://github.com/ktgroth/k-means.git\n\n"
        "k-means 1.0.0 2025-07-01\n\n");
        return 0;
    }

    else
    {
        for (int i = 1; i < argc; ++i)
        {
            if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
            {
                fprintf(stderr, "Incorrect argument inputs\n");
                return 1;
            }

            if (!strcmp(argv[i], "-o") || !strncmp(argv[i], "--output=", strlen("--output=")))
            {
                if (!strcmp(argv[i], "-o"))
                {
                    if (++i >= argc)
                    {
                        fprintf(stderr, "-o expected filename\n");
                        return 1;
                    }
                    ofilename = argv[i];
                } else
                {
                    ofilename = argv[i] + strlen("--output=");
                }
            } else if (!strcmp(argv[i], "-i") || !strncmp(argv[i], "--input=", strlen("--input=")))
            {
                if (!strcmp(argv[i], "-i"))
                {
                    if (++i >= argc)
                    {
                        fprintf(stderr, "-i expected filename\n");
                        return 1;
                    }
                    ifilename = argv[i];
                } else 
                {
                    ifilename = argv[i] + strlen("--input=");
                }
            } else if (!strcmp(argv[i], "-n") || !strncmp(argv[i], "--genn=", strlen("--genn=")))
            {
                if (!strcmp(argv[i], "-n"))
                {
                    if (++i >= argc)
                    {
                        fprintf(stderr, "-n expected integer");
                        return 1;
                    }
                    n = atoi(argv[i]);
                } else
                {
                    n = atoi(argv[i] + strlen("--genn="));
                }
            } else if (!strcmp(argv[i], "-d") || !strncmp(argv[i], "--dim=", strlen("--dim=")))
            {
                if (!strcmp(argv[i], "-d"))
                {
                    if (++i >= argc)
                    {
                        fprintf(stderr, "-d expected interger");
                        return 1;
                    }
                    d = atoi(argv[i]);
                } else
                {
                    d = atoi(argv[i] + strlen("--dim="));
                }
            } else if (!strcmp(argv[i], "-k") || !strncmp(argv[i], "--groups=", strlen("--groups=")))
            {
                if (!strcmp(argv[i], "-k"))
                {
                    if (++i >= argc)
                    {
                        fprintf(stderr, "-k expected integer");
                        return 1;
                    }
                    k = atoi(argv[i]);
                } else
                {
                    k = atoi(argv[i] + strlen("--groups="));
                }
                }
            }
        }
    }

    if (!ofilename)
        ofilename = "clusterings/k-clusters";
    if (!k)
    {
        fprintf(stderr, "Number of Groups (K) needed!\n");
        return 1;
    }

    if (n && ifilename)
    {
        fprintf(stderr, "Use one means of input\n");
        return 1;
    }

    point_t **points;
    if (ifilename)
    {
        point_array_t pinfo = read_points(ifilename);
        points = pinfo.points;
        n = pinfo.size;
    } else
    {
        if (d == 0)
        {
            fprintf(stderr, "Cannot generate points of dimension 0\n");
            return 1;
        }
        points = generate_points(k, n, d);
    }

    group_t *groups = cluster(k, n, points);
    size_t *counts = (size_t *)calloc(k, sizeof(size_t));

    FILE *ofile = fopen(ofilename, "w+");
    for (size_t i = 0; i < k; ++i)
    {
        for (size_t j = 1; j < groups[i].size; ++j)
        {
            point_t *point = groups[i].points[j];
            for (size_t l = 0; l < point->dim; ++l)
                fprintf(ofile, "%-3.6lf\t", point->components[l]);
            fprintf(ofile, "%zu\n", i+1);

            ++counts[i];
        }
        fprintf(ofile, "\n\n");
    }

    for (size_t i = 0; i < k; ++i)
    {
        for (size_t j = 0; j < groups[i].size; ++j)
        {
            free(groups[i].points[j]->components);
            free(groups[i].points[j]);
        }
        free(groups[i].points);
    }
    free(points);
    free(groups);
    fclose(ofile);


    FILE * script = fopen("scripts/plot_script.gp", "w");
    if (!script)
    {
        perror("fopen(\"scripts/plot_script.gp\", \"w\")");
        return 1;
    }

    fprintf(script,
            "set title 'K-means Clustering (%zu Points   %zu Clusters)'\n"
            "set xlabel 'X'\n"
            "set ylabel 'Y'\n"
            "set zlabel 'Z'\n"
            "unset colorbox\n"
            "set key outside right center\n"
            "set palette defined (1 'red', 2 'blue', 3 'green', 4 'purple', 5 'orange', 6 'cyan')\n"
            "set style fill solid 1.0\n\n",
            n, k);

    for (size_t i = 0; i < k; ++i)
        fprintf(script,
                "$c%zu <<EOD\n1 1 %zu\nEOD\n",
                i, i+1);

    fprintf(script, d >= 3 ? "splot \\\n" : "plot \\\n");
    for (size_t i = 0; i < k; ++i)
    {
        fprintf(script,
                "$c%zu using 1:2:3 with points pt 7 ps 1.5 lc palette lw 0x00001 title 'Cluster %zu (%zu points)', \\\n",
                i, i+1, counts[i]);
    }

    fprintf(script,
            "'%s' using ",
            ofilename);

    if (d >= 3)
        fprintf(script,
                "1:2:3:%ld ",
                d+1);
    else
        fprintf(script,
                "1:2:%ld ",
                d+1);

    fprintf(script,
            "with points pt 7 ps 1.5 lc palette notitle");

    fclose(script);
    return 0;
}
