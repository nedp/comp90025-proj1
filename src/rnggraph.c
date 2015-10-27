#include "rnggraph.h"

const int BIG_POWER_OF_TWO = 2 << (sizeof(int)*8 - 3);

unsigned int current_seed;

/*
 * Resets the seed for randomly generated graphs.
 *
 * The same series of `set_seed` and `generate_graph` calls
 * should produce the same series of graph reliably,
 * but not when used in parallel.
 */
void set_seed(unsigned int seed)
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
void generate_graph(unsigned int size,
                    float b,
                    unsigned int max_weight,
                    int *edges)
{
    srand(current_seed);

    int bint = b * BIG_POWER_OF_TWO;

    for (int i = 0; i < (size*size); i += 1) {
        const bool should_add_edge = (rand() % BIG_POWER_OF_TWO) < bint;
        edges[i] = (should_add_edge) ? rand() % (max_weight+1) : -1;
    }

    current_seed = rand();
}

#if 0
int
main(int argc, char** argv)
{
    set_seed(time(NULL));

    int edges[1];
    generate_graph(1, 1, 0, edges);

    return 0;
}
#endif // 0
