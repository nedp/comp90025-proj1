# Parallel Dijkstra's Algorithm

## COMP90025 Project 1

A parallel program which randomly generates a graph, then runs
Dijkstra's algorithm on it.

## Testing

The serial version of the code can be be tested with Ceedling
and its Unity testing framework:

1. Install the ceedling gem: `$ gem install ceedling`
2. Set up the ceedling environment: `$ ceedling new .`
3. Run the tests: `$ rake test:all`

## Building

`$ make pdijkstra`

## Running

`$ target/pdijkstra <size> <b> <max_weight> <seed> <nthreads>`

| Parameter   | Description |
|------------:|:------------|
|      size   | Number of vertices in the generated graph. |
|         b   | Probability (0.0 to 1.0) that a given pair of vertices will have an edge. |
| max\_weight | Maximum edge weight. |
|      seed   | Seed to use for randomly generating the graph. |
|   nthreads  | Number of threads to use. |
