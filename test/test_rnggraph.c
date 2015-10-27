#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "rnggraph.h"

#define TEST_MAX_GRAPH_SIZE (100)
#define TEST_MAX_EDGES_SIZE (TEST_MAX_GRAPH_SIZE * TEST_MAX_GRAPH_SIZE)

/**
 * NOTE! THESE TESTS ARE BASED ON RNG!
 * THERE ARE SOMETIMES FALSE NEGATIVES!
 */

unsigned int seed;

int edges[TEST_MAX_EDGES_SIZE];
unsigned int size;
float b;
unsigned int max_weight;

static void expect_reasonable_edges(void);
static void expect_valid_edges(void);
int want[TEST_MAX_EDGES_SIZE];

void setUp(void)
{
    seed = time(NULL);
    set_seed(seed);

    for (int i = 0; i < TEST_MAX_EDGES_SIZE; i += 1) {
        want[i] = edges[i];
    }
}

void tearDown(void)
{
}

void test_callable(void)
{
    set_seed(0);
    generate_graph(1, 0.5, 5, edges);
}

// Equivalence classes:
// * size = 1, size = N
// * b = 0.0, b = 1.0, b <- (0.0)..(1.0)
// * max_weight = 0, max_weight = N

void test_one_zero_zero(void)
{
    size = 1;
    b = 0.0;
    max_weight = 0;

    want[0] = -1;

    generate_graph(size, b, max_weight, edges);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, edges, TEST_MAX_EDGES_SIZE);
}

// Note: the one_zero_one case is meaningless.

void test_one_one_zero(void)
{
    size = 1;
    b = 1.0;
    max_weight = 0;

    want[0] = 0;

    generate_graph(size, b, max_weight, edges);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, edges, TEST_MAX_EDGES_SIZE);
}

void test_one_one_one(void)
{
    size = 1;
    b = 1.0;
    max_weight = 1;

    generate_graph(size, b, max_weight, edges);

    expect_valid_edges();
}

void test_fifty_zero_zero(void)
{
    size = TEST_MAX_GRAPH_SIZE / 2;
    b = 0.0;
    max_weight = 0;

    generate_graph(size, b, max_weight, edges);

    expect_reasonable_edges();
}

void test_fifty_zero_five(void)
{
    size = TEST_MAX_GRAPH_SIZE / 2;
    b = 0.0;
    max_weight = 5;

    generate_graph(size, b, max_weight, edges);

    expect_reasonable_edges();
}

void test_fifty_one_zero(void)
{
    size = TEST_MAX_GRAPH_SIZE / 2;
    b = 1.0;
    max_weight = 0;

    generate_graph(size, b, max_weight, edges);

    expect_reasonable_edges();
}

void test_fifty_one_five(void)
{
    size = TEST_MAX_GRAPH_SIZE / 2;
    b = 1.0;
    max_weight = 5;

    generate_graph(size, b, max_weight, edges);

    expect_reasonable_edges();
}

void test_fifty_half_zero(void)
{
    size = TEST_MAX_GRAPH_SIZE / 2;
    b = 0.5;
    max_weight = 0;

    generate_graph(size, b, max_weight, edges);

    expect_reasonable_edges();
}

void test_fifty_half_five(void)
{
    size = TEST_MAX_GRAPH_SIZE / 2;
    b = 0.5;
    max_weight = 5;

    generate_graph(size, b, max_weight, edges);

    expect_reasonable_edges();
}

void test_same_seed(void)
{
    size = TEST_MAX_GRAPH_SIZE - 1;
    b = 0.5;
    max_weight = 100;

    generate_graph(size, b, max_weight, edges);

    memcpy(want, edges, sizeof(want));

    set_seed(seed);
    generate_graph(size, b, max_weight, edges);
    TEST_ASSERT_EQUAL_INT_ARRAY(want, edges, TEST_MAX_EDGES_SIZE);
}

void test_different_seed(void)
{
    size = TEST_MAX_GRAPH_SIZE - 1;
    b = 0.5;
    max_weight = 100;

    generate_graph(size, b, max_weight, edges);

    memcpy(want, edges, sizeof(want));

    set_seed(seed + 1);
    generate_graph(size, b, max_weight, edges);

    const bool were_the_same = (0 == memcmp(want, edges, sizeof(edges)));
    TEST_ASSERT_FALSE_MESSAGE(were_the_same, "different seeds should produce different edges");
}

static inline void
expect_reasonable_edges(void)
{
    bool did_have_weights[max_weight];
    for (int i = 0; i < max_weight; i += 1)
        did_have_weights[i] = false;

    bool did_have_non_edge = false;
    bool did_have_edge = false;
    for (int i = 0; i < size; i += 1) {
        if (edges[i] == -1) {
            did_have_non_edge = true;
        } else {
            did_have_edge = true;
            if (edges[i] < 0 || edges[i] > max_weight) {
                printf("invalid edge weight: %d\n", edges[i]);
                TEST_FAIL_MESSAGE("invalid edge weight");
            } else {
                did_have_weights[edges[i]] = true;
            }
        }
    }

    if (b == 1.0) {
        TEST_ASSERT_FALSE_MESSAGE(did_have_non_edge, "expected NO non-edges");
    } else {
        TEST_ASSERT_TRUE_MESSAGE(did_have_non_edge, "expected atleast one non-edge");
    }

    if (b == 0.0) {
        TEST_ASSERT_FALSE_MESSAGE(did_have_edge, "expected NO edges");
    } else {
        TEST_ASSERT_TRUE_MESSAGE(did_have_edge, "expected atleast one edge");
    }

    if (b != 0.0) {
        for (int i = 0; i < max_weight; i += 1)
            TEST_ASSERT_TRUE_MESSAGE(did_have_weights[i],
                    "expected atleast one edge of each weight");
    }
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(want + size*size, edges + size*size,
            TEST_MAX_EDGES_SIZE - size*size, "expected the rest of the buffer to be untouched");
}

static inline void
expect_valid_edges(void)
{
    bool did_have_edge = false;
    bool did_have_non_edge = false;
    for (int i = 0; i < size; i += 1) {
        if (edges[i] < -1 || edges[i] > max_weight) {
            printf("invalid edge weight: %d\n", edges[i]);
            TEST_FAIL_MESSAGE("invalid edge weight");
        }
        if (edges[i] == -1) did_have_non_edge = true;
        else did_have_edge = true;
    }

    if (b == 1.0) TEST_ASSERT_FALSE_MESSAGE(did_have_non_edge, "expected NO non-edges");
    if (b == 0.0) TEST_ASSERT_FALSE_MESSAGE(did_have_edge, "expected NO edges");

    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(want + size*size, edges + size*size,
            TEST_MAX_EDGES_SIZE - size*size, "expected the rest of the buffer to be untouched");
}
