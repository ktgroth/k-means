
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <complex.h>

#include "include/point.h"
#include "include/k-means.h"

#define SPREAD 5.0


typedef struct
{
    point_t *points;
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

    size_t i = 0;
    point_t *points = NULL;
    while ((read = getline(&line, &n, ifile)) != -1)
    {
        double x, y;
        if (sscanf(line, "%lf %lf", &x, &y) == 2)
        {
            points = (point_t *)realloc(points, ++i * sizeof(point_t));
            points[i - 1] = CMPLX(x, y);
        }
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

point_t *generate_points(size_t k, size_t n)
{
    srand(time(NULL));

    size_t t = 0;
    point_t *points = NULL;
    for (size_t i = 0; i < k; ++i) {
        double x = rand_double(-50., 50), y = rand_double(-50., 50.);
        for (size_t j = 0; t < n && j < (size_t)ceil((double)n / k); ++j, ++t)
        {
            double dx = rand_double(-SPREAD, SPREAD), dy = rand_double(-SPREAD, SPREAD);
            
            points = (point_t *)realloc(points, (t + 1) * sizeof(point_t));
            points[t] = CMPLX(x+dx, y+dy);
        }
    }

    return points;
}

int main(int argc, char *argv[])
{
    char *ifilename = NULL, *ofilename = NULL;
    size_t k = 0, n = 0;

    if (argc < 2)
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
            "\t-k, --groups=K: K groups of points.\n\n");
        return 1; 
    } else {
    if (!strcmp(argv[1], "--help"))
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
            "\t-k, --groups=K: K groups of points.\n\n");
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
        ofilename = "k-clusters";
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

    printf("%ld %ld\n", k, n);
    point_t *points;
    if (ifilename)
    {
        point_array_t pinfo = read_points(ifilename);
        points = pinfo.points;
        n = pinfo.size;
    } else
    {
        points = generate_points(k, n);
    }

    group_t *groups = cluster(k, n, points);
    free(points);

    FILE *ofile = fopen(ofilename, "w+");
    for (size_t i = 0; i < k; ++i)
    {
        for (size_t j = 1; j < groups[i].size; ++j)
            fprintf(ofile, "%lf %lf %zu\n", creal(groups[i].points[j]), cimag(groups[i].points[j]), i);
    }

    for (size_t i = 0; i < k; ++i)
        free(groups[i].points);

    free(groups);

    fclose(ofile);

    system("gnuplot -p -e \"set title 'K-means Clustering'; "
           "set xlabel 'X'; set ylabel 'Y'; "
           "set key off; "
           "set palette model RGB defined (0 'red', 1 'blue', 2 'green', 3 'orange', 4 'purple'); "
           "set style fill solid 1.0; "
           "plot 'k-clusters' using 1:2:3 with points pt 7 ps 1.5 lc palette notitle \"");

    return 0;
}
