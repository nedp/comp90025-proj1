#ifndef rnggraph_H
#define rnggraph_H

/**
 * @file
 * Generator for graphs with edges randomly determined.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/**
 * Resets the seed for randomly generated graphs.
 *
 * The same series of `set_seed` and `generate_graph` calls
 * should produce the same series of graph reliably,
 * but not when used in parallel.
 */
void set_seed(unsigned int seed);

/**
 * Randomly generates a graph of the specified size,
 * branching factor, and maximum edge weight.
 *
 * @param size  the number of vertices in the graph; positive.
 *
 * @param b  the branching factor; probability that any given
 * source destination pair will have an edge.
 *
 * @param max_weight  the upper bound edge weight; each edge
 * has a randomly chosen non-negative weight at most this.
 */
void generate_graph(unsigned int size,
                    float b,
                    unsigned int max_weight,
                    int *edges);

#endif // rnggraph_H
