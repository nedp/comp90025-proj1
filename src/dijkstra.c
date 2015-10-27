#include "dijkstra.h"

static inline void prepare_buffers(int, int, int*, int*, bool*, bool*);
static inline int nearest_vertex(int, bool const*, bool const*, int const*);
static inline void visit_vertex(int, int const*, int, bool*, int*, int*);

/*
 * Applies Dijkstra's algorithm to the input graph
 * with the specified source.
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
dijkstra(int const*edges, unsigned int size, unsigned int source, int *paths)
{
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
 */
static inline void
prepare_buffers(int size, int source, int *distances, int *paths,
        bool *seen_set, bool *visited_set)
{
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
    int v;
    // Find the first valid vertex.
    for (v = 0; v < size; v += 1)
        if (seen_set[v] && !visited_set[v]) break;

    // Find the closest valid vertex.
    for (int u = 0; u < size; u += 1)
        if (!visited_set[u] && seen_set[u] && (distances[u] < distances[v]))
            v = u;

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
    for (int w = 0; w < size; w += 1) {
        if (edges[v*size + w] == -1) continue;
        seen_set[w] = true;

        if (distances[w] > distances[v] + edges[v*size + w]) {
            distances[w] = distances[v] + edges[v*size + w];
            paths[w] = v;
        }
    }
}
