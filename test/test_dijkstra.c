#include<stdlib.h>
#include<stdio.h>
#include <stdbool.h>

#include "unity.h"
#include "dijkstra.h"

#define TEST_MAX_GRAPH_SIZE (10)

int edges[TEST_MAX_GRAPH_SIZE * TEST_MAX_GRAPH_SIZE] = {0};
int paths[TEST_MAX_GRAPH_SIZE * TEST_MAX_GRAPH_SIZE] = {0};
int size;
int source;
int want[TEST_MAX_GRAPH_SIZE * TEST_MAX_GRAPH_SIZE] = {0};

void setUp(void)
{
    memcpy(want, paths, sizeof(edges));

    for (int i = 0; i < TEST_MAX_GRAPH_SIZE * TEST_MAX_GRAPH_SIZE; i += 1)
        edges[i] = -1;

    for (int i = 2; i < TEST_MAX_GRAPH_SIZE; i += 1)
        want[i] = paths[i];
}

void tearDown(void)
{
}

void test_single_node(void)
{
    size = 1;
    source = 0;

    want[0] = 0;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_two_nodes_no_edge_source_zero(void)
{
    size = 2;
    source = 0;

    want[0] = 0;
    want[1] = -1;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_two_nodes_forward_edge_source_zero(void)
{
    size = 2;
    edges[0*size + 1] = 4;
    source = 0;

    want[0] = 0;
    want[1] = 0;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_two_nodes_forward_edge_source_one(void)
{
    size = 2;
    edges[0*size + 1] = 4;
    source = 1;

    want[0] = -1;
    want[1] = 1;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_two_nodes_backward_edge_source_one(void)
{
    size = 2;
    edges[1*size + 0] = 4;
    source = 1;

    want[0] = 1;
    want[1] = 1;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_two_nodes_loop_source_zero(void)
{
    size = 2;
    edges[1*size + 0] = 4;
    edges[0*size + 1] = 4;
    source = 0;

    want[0] = 0;
    want[1] = 0;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_five_nodes_no_edge_source_zero(void)
{
    size = 5;
    source = 0;

    want[0] = 0;
    want[1] = -1;
    want[2] = -1;
    want[3] = -1;
    want[4] = -1;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_five_nodes_forward_edges_source_zero(void)
{
    size = 5;
    edges[0*size + 1] = 0;
    edges[1*size + 2] = 0;
    edges[2*size + 3] = 0;
    edges[3*size + 4] = 0;
    source = 0;

    want[0] = 0;
    want[1] = 0;
    want[2] = 1;
    want[3] = 2;
    want[4] = 3;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_five_nodes_forward_edge_source_two(void)
{
    size = 5;
    edges[0*size + 1] = 0;
    edges[1*size + 2] = 0;
    edges[2*size + 3] = 0;
    edges[3*size + 4] = 0;
    source = 2;

    want[0] = -1;
    want[1] = -1;
    want[2] = 2;
    want[3] = 2;
    want[4] = 3;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_five_nodes_backward_edges_source_two(void)
{
    size = 5;
    edges[1*size + 0] = 0;
    edges[2*size + 1] = 0;
    edges[3*size + 2] = 0;
    edges[4*size + 3] = 0;
    source = 2;

    want[0] = 1;
    want[1] = 2;
    want[2] = 2;
    want[3] = -1;
    want[4] = -1;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_five_nodes_loop_source_two(void)
{
    size = 5;
    edges[0*size + 1] = 0;
    edges[1*size + 2] = 0;
    edges[2*size + 3] = 0;
    edges[3*size + 4] = 0;
    edges[4*size + 0] = 0;
    source = 2;

    want[0] = 4;
    want[1] = 0;
    want[2] = 2;
    want[3] = 2;
    want[4] = 3;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_lower_path(void)
{
    size = 5;
    edges[0*size + 1] = 3;
    edges[0*size + 2] = 4;
    edges[1*size + 3] = 0;
    edges[2*size + 3] = 0;
    edges[3*size + 4] = 0;
    source = 0;

    want[0] = 0;
    want[1] = 0;
    want[2] = 0;
    want[3] = 1;
    want[4] = 3;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_upper_path(void)
{
    size = 5;
    edges[0*size + 1] = 4;
    edges[0*size + 2] = 3;
    edges[1*size + 3] = 0;
    edges[2*size + 3] = 0;
    edges[3*size + 4] = 0;
    source = 0;

    want[0] = 0;
    want[1] = 0;
    want[2] = 0;
    want[3] = 2;
    want[4] = 3;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}

void test_upper_path_complex(void)
{
    size = 5;
    edges[0*size + 1] = 2;
    edges[0*size + 2] = 4;
    edges[1*size + 3] = 5;
    edges[2*size + 3] = 2;
    edges[3*size + 4] = 0;
    source = 0;

    want[0] = 0;
    want[1] = 0;
    want[2] = 0;
    want[3] = 2;
    want[4] = 3;

    dijkstra(edges, size, source, paths);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, paths, TEST_MAX_GRAPH_SIZE);
}
