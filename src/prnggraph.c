#include "prnggraph.h"

unsigned int current_seed;

const static int BIG_POWER_OF_TWO = 2 << (sizeof(int)*8 - 3);

/*
 * Resets the seed for randomly generated graphs.
 *
 * The same series of `set_seed` and `generate_graph` calls
 * should produce the same series of graph reliably,
 * but not when used in parallel.
 */
void pset_seed(unsigned int seed)
{
    current_seed = seed;
}

/*
 * Randomly generates a graph of the specified size,
 * branching factor, and maximum edge weight.
 *
 * @param size  the number of vertices in the graph; positive.
 *
 * @param b  the branching factor; probability that any given
 * source destination pair will have an edge.
 *
 * @param max_weight  the upper bound edge weight; each edge
 * has a pseudorandomly chosen non-negative weight at most this.
 * Must be a power of 2 less than RAND_MAX for a fair weight
 * distribution.
 */
void pgenerate_graph(unsigned int size,
                    float b,
                    unsigned int max_weight,
                    int *edges)
{
    int bint = b * (double) BIG_POWER_OF_TWO;

#pragma omp parallel
    {
        struct drand48_data buffer;
        srand48_r(current_seed + omp_get_thread_num(), &buffer);

#pragma omp for
        for (int i = 0; i < (size*size); i += 1) {
            long rng;
            lrand48_r(&buffer, &rng);
            const bool should_add_edge = ((int) rng % BIG_POWER_OF_TWO) < bint;
            edges[i] = (should_add_edge) ? (int) rng % (max_weight+1) : -1;
        }
    }
    srand(current_seed);
    current_seed = rand();
}
