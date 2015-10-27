#include <omp.h>

#include "pdijkstra.h"

static inline void prepare_buffers(int, int, int*, int*, bool*, bool*);
static inline int nearest_vertex(int, bool const*, bool const*, int const*);
static inline void visit_vertex(int, int const*, int, bool*, int*, int*);
static inline int thread_nearest_vertex(int, int const*, bool const*, bool const*);

// Uncomment for unity tests.
// static inline int omp_get_max_threads() { return 1; }
// static inline int omp_get_thread_num() { return 0; }

/*
 * Applies Dijkstra's algorithm to the input graph
 * with the specified source, using parallelisation.
 *
 * Parallelisation:
 *
 *  1. The buffer initialisation is parallelised by having
 *     each processor initialise a subset of the buffer.
 *  2. Finding the nearest vertex is parallelised by using
 *     the parallel min algorithm discussed in lectures.
 *  3. Extending the visited_set is done by having each
 *     processor check neighbour/path details for a subset
 *     of the vertices.
 *
 * @param edges  a buffer containing a matrix of edge weights.
 * `edges[v*size + w]` should be the weight of the edge from
 * the vertex v to the vertex w, with -1 meaning no edge.
 *
 * @param size  the number of nodes in the graph; positive.
 *
 * @param source  the id of the node to visit as the source for the
 * algorithm; non-negative, less than size.
 *
 * @param paths  the buffer in which to place the paths.
 *
 * Paths are repesented by storing each node's predecessor
 * in the specified buffer, with node ids as the indices.
 */
void
pdijkstra(int const*edges, unsigned int size, unsigned int source, int *paths)
{
    // Can't parallise this function I think... only its subroutines.
    bool seen_set[size];
    bool visited_set[size];
    int distances[size];
    prepare_buffers(size, source, distances, paths, seen_set, visited_set);

    // For (at most) every vertex:
    for (int nvisited = 0; nvisited < size; nvisited += 1) {
        int v = nearest_vertex(size, seen_set, visited_set, distances);
        if (v == size) break; // No more seen but unvisited vertices.
        visited_set[v] = true;

        visit_vertex(v, edges, size, seen_set, distances, paths);
    }
}

/*
 * Mark all vertexs as unseen, unvisited, at infinite distance,
 * and having no path to the source.
 * Begin with the seen set being just the source.
 *
 * Parallelise by making each processer initialise
 * a `p`th of the elements.
 */
inline static void
prepare_buffers(int size, int source, int *distances, int *paths,
        bool *seen_set, bool *visited_set)
{
#pragma omp parallel for
    for (int i = 0; i < size; i += 1) {
        distances[i] = INT_MAX;
        paths[i] = -1;
        seen_set[i] = false;
        visited_set[i] = false;
    }
    seen_set[source] = true;
    distances[source] = 0;
    paths[source] = source;
}

/*
 * Pick the unvisited, but seen vertex with the shortest
 * path from the source.
 * Returns `size` if there are no valid vertices.
 */
static inline int
nearest_vertex(int size, bool const*seen_set, bool const*visited_set,
        int const*distances)
{
    // Parallelise by using the paralel min algorithm.
    int v = size;
    int vdistance = INT_MAX;

    // Have each thread handle a fairly sized chunk of the input.
#pragma omp parallel shared(v, vdistance)
    {
        const int my_v = thread_nearest_vertex(size, distances, visited_set, seen_set);

        const int my_vdistance = distances[my_v];

        // Reduce the chunks using `min`.
#pragma omp critical
        {
            if (my_v < size && my_vdistance < vdistance) {
                vdistance = my_vdistance;
                v = my_v;
            }
        }
    }

    return v;
}

static inline int
thread_nearest_vertex(int size, int const*distances, bool const*visited_set,
        bool const*seen_set)
{
    const int nthreads = omp_get_num_threads();
    const int ithread = omp_get_thread_num();
    const int min = ithread * (size / nthreads);
    // Exclusive max.
    // Last thread should fill out the remaining vertices.
    const int max = (ithread < (nthreads-1))
        ? (ithread+1) * (size / nthreads) : size;
    int v = size; // Return value for function failure.
    int vdistance = INT_MAX;

    // Find the closest valid vertex in my chunk.
    for (int u = min; u < max; u += 1) {
        if (!visited_set[u] && seen_set[u] && (distances[u] < vdistance)) {
            v = u;
            vdistance = distances[v];
        }
    }

    return v;
}

/*
 * Check each of v's neighbours, w, marking them as seen.
 * If the path to w through v is shorter than the previous
 * shortest known path, remember it.
 */
static inline void
visit_vertex(int v, int const*edges, int size,
        bool *seen_set, int *distances, int*paths)
{
    // Parallelise by having each processer check one
    // `p`th of the vertices as `w`.
#pragma omp parallel for
    for (int w = 0; w < size; w += 1) {
        if (edges[v*size + w] == -1) continue;
        seen_set[w] = true;

        if (distances[w] > distances[v] + edges[v*size + w]) {
            distances[w] = distances[v] + edges[v*size + w];
            paths[w] = v;
        }
    }
}
