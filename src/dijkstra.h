#ifndef dijkstra_H
#define dijkstra_H

/**
 * @file
 * Serial implementation of Dijkstra's algorithm using graphs
 * defined by weighted adjacency matrices.
 */

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Applies Dijkstra's algorithm to the input graph
 * with the specified source.
 *
 * @param edges  a buffer containing a matrix of edge weights.
 * `edges[v*size + w]` should be the weight of the edge from
 * the vertex v to the vertex w, with -1 meaning no edge.
 *
 * @param size  the number of nodes in the graph; positive.
 *
 * @param source  the id of the node to use as the source for the
 * algorithm; non-negative, less than size.
 *
 * @param paths  the buffer in which to place the paths.
 * Paths are repesented by storing each node's predecessor
 * in the specified buffer, with node ids as the indices.
 */
void dijkstra(int const* edges,
              unsigned int size,
              unsigned int source,
              int * paths);

#endif // dijkstra_H
