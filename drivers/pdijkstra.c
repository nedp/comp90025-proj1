#include <omp.h>
#include <time.h>

#include "../src/pdijkstra.h"
#include "../src/prnggraph.h"

int
main(int argc, char **argv)
{

    const unsigned int size = atoi(argv[1]);
    const float b = atof(argv[2]);
    const unsigned int max_weight = atoi(argv[3]);
    const unsigned int nthreads = atoi(argv[4]);

    // printf("size: %u\n", size);
    // printf("b: %f\n", b);
    // printf("max_weight: %u\n", max_weight);
    // printf("seed: %lu\n", seed);
    // printf("nthreads: %d\n", nthreads);

    omp_set_num_threads(nthreads);

    int *edges = (int*) malloc(size*size * sizeof(int));
    int *paths = (int*) malloc(size * sizeof(int));

    // This driver uses predetermined rng seeds for consistent results.
    for (int seed = 0; seed < 10; seed += 1) {
        {
            printf("----\n"
                   "seed: %d\n", seed);

            printf("Generating the graph...");
            fflush(stdout);
            const double start_wall = omp_get_wtime();
            const long start_cpu = clock();
            pset_seed(seed);
            pgenerate_graph(size, b, max_weight, edges);
            printf("... Done\n"
                   "time: %fs\n"
                   "work: %ld ticks\n",
                   omp_get_wtime() - start_wall,
                   clock() - start_cpu);
        }

        {
            printf("Running the algorithm...");
            fflush(stdout);
            const double start_wall = omp_get_wtime();
            const long start_cpu = clock();
            pdijkstra(edges, size, 0, paths);
            printf("... Done\n"
                   "time: %fs\n"
                   "work: %ld ticks\n",
                   omp_get_wtime() - start_wall,
                   clock() - start_cpu);
        }
    }

    free(edges);
}
