#include <stdlib.h>
#include "algorithms.h"


/* Sort the elements of `array` in ascending order.
 *
 *   Idea: Find the smallest number and swap it with the first element. Find the second smallest
 *   number and swap it with the second element, and so on until the list is sorted.
 *
 *   Time analysis: The outer loop runs n times, so n swaps are made. When i=0, the inner loop
 *   runs n-1 times, when i=1 n-2 times, and so on for a total of n-1 + n-2 + ... + 1 times,
 *   which equals n(n-1)/2. This is asymptotically equivalent to n^2, so O(n^2) comparisons are
 *   made. Therefore the overall time complexity is O(n^2).
 *
 *   Space analysis: No extra space is used, so O(1).
 */
void selection_sort(int array[], size_t n) {
    /* On each run of this loop, the smallest number in array[i...n] is found and swapped with
     * array[i].
     */
    for (size_t i = 0; i < n; i++) {
        int minimum = array[i];
        size_t minimum_pos = i;
        /* This loop finds the smallest number in array[i...n]. */
        for (size_t j = i+1; j < n; j++) {
            if (array[j] < minimum) {
                minimum = array[j];
                minimum_pos = j;
            }
        }
        swap(array, i, minimum_pos);
    }
}


/* Return the first position of `datum` in the array, or -1 if `datum` is not present.
 *
 *   Idea: Examine each element in turn.
 *
 *   Time analysis: In the worst case, the datum is not in the array and every element is examined,
 *   so O(n).
 *
 *   Space analysis: O(1).
 */
long long linear_search(int array[], size_t n, int datum) {
    for (size_t i = 0; i < n; i++) {
        if (array[i] == datum)
            return i;
    }
    return -1;
}


/* Given the two lists of the same set of unique points, one in ascending order of the x-coordinate
 * and the other in ascending order of the y-coordinate, return the distance between the two closest
 * points.
 *
 *   Idea: Compute the distance between each pair of points and keep track of the minimum distance
 *   seen so far.
 *
 *   Time analysis: The nested for loops consider roughly n^2 pairs of points, so O(n^2).
 *
 *   Space analysis: O(1).
 */
double closest_pair_brute_force(Point points[], size_t n) {
    if (n >= 2) {
        double closest_so_far = distance_squared(points[0], points[1]);
        for (size_t i = 0; i < n-1; i++) {
            for (size_t j = i+1; j < n; j++) {
                double this_distance = distance_squared(points[i], points[j]);
                if (this_distance < closest_so_far) {
                    closest_so_far = this_distance;
                }
            }
        }
        return sqrt(closest_so_far);
    } else {
        return 0.0;
    }
}


/* Traverse the graph depth-first and return a heap-allocated array indicating the order in which
 * each vertex was visited, starting at 1, so if A[7] = 2 then the eighth vertex (g->vertices[7])
 * was the second vertex visited.
 *
 *  Idea: Mark each vertex of the graph with 0. Visit the first vertex, mark it with 1, then visit
 *  that vertex's first (unmarked) neighbor, mark it with 2, and continue until you reach a dead
 *  end. Backtrack to a vertex with unmarked neighbors and continue from there.
 *
 *  Time analysis: Every vertex must be visited, and each edge of every vertex must be considered,
 *  so the time complexity is O(|E| + |V|).
 *
 *  Space analysis: O(|V|) for the counts array and vertex stack.
 */
int* depth_first_search(const Graph* g) {
    if (g == NULL) return NULL;
    VertexStack* stack = stack_new(g->n);
    int* counts = safe_calloc(g->n, sizeof *counts);
    int max_count = 0;
    /* Start at each vertex (this ensures that every component is visited). */
    for (size_t i = 0; i < g->n; i++) {
        if (counts[i] > 0)
            continue;
        stack_push(stack, g->vertices + i);
        while (!stack_empty(stack)) {
            Vertex* this_vertex = stack_pop(stack);
            size_t this_index = this_vertex - g->vertices;
            if (counts[this_index] == 0) {
                counts[this_index] = ++max_count;
                VertexList* p = this_vertex->neighbors;
                /* Push all adjacents vertices onto the stack. */
                while (p != NULL) {
                    stack_push(stack, p->v);
                    p = p->next;
                }
            }
        }
    }
    stack_free(stack);
    return counts;
}


/* Traverse the graph breadth-first and return a heap-allocated array indicating the order in which
 * each vertex was visited, starting at 1, so if A[7] = 2 then the eighth vertex (g->vertices[7])
 * was the second vertex visited.
 *
 *  Idea: Same idea as depth-first search, except use a queue instead of a stack so that every
 *  neighbor of a vertex is visited before any other vertex is.
 *
 *  Time analysis: Every vertex must be visited, and each edge of every vertex must be considered,
 *  so the time complexity is O(|E| + |V|).
 *
 *  Space analysis: O(|V|) for the counts array and vertex queue.
 */
int* breadth_first_search(const Graph* g) {
    VertexQueue* queue = queue_new(g->n);
    int* counts = safe_calloc(g->n, sizeof *counts);
    int max_count = 0;
    /* Start at each vertex (this ensures that every component is visited). */
    for (size_t i = 0; i < g->n; i++) {
        if (counts[i] > 0)
            continue;
        queue_push(queue, g->vertices + i);
        while (!queue_empty(queue)) {
            Vertex* this_vertex = queue_pop(queue);
            size_t this_index = this_vertex - g->vertices;
            if (counts[this_index] == 0) {
                counts[this_index] = ++max_count;
                VertexList* p = this_vertex->neighbors;
                /* Push all adjacents vertices onto the stack. */
                while (p != NULL) {
                    queue_push(queue, p->v);
                    p = p->next;
                }
            }
        }
    }
    queue_free(queue);
    return counts;
}


int ch03_tests() {
    puts("\n=== CHAPTER 3 TESTS ===");
    int tests_failed = 0;

    /* SELECTION SORT */
    puts("Testing selection sort");
    ASSERT(test_sorting_f(selection_sort) == 0);

    /* LINEAR SEARCH */
    puts("Testing linear search");
    int ls_data[] = {1, 2, 3, 4, 3};
    ASSERT(linear_search(ls_data, 5, 3) == 2);
    ASSERT(linear_search(ls_data, 5, 1) == 0);
    ASSERT(linear_search(ls_data, 5, 7) == -1);

    /* BRUTE-FORCE CLOSEST PAIR */
    puts("Testing brute-force closest pair");
    Point points[] = { {7, 3}, {7, 1}, {2, 3}, {3, 1} };
    ASSERT(closest_pair_brute_force(points, 4) == 2);

    /* DEPTH-FIRST SEARCH */
    puts("Testing depth-first search");
    Graph* g = graph_from_string(DIRECTED, "ABCDEFG", "AB AC BG BE CF DA DB DC DF DG GF");
    int* counts = depth_first_search(g);
    /* Expected order: A, B, G, F, E, C, D */
    ASSERT(array_eq(7, counts, 1, 2, 6, 7, 5, 4, 3));
    free(counts);

    /* BREADTH-FIRST SEARCH */
    puts("Testing breadth-first search");
    counts = breadth_first_search(g);
    /* Expected order: A, C, B, F, E, G, D */
    ASSERT(array_eq(7, counts, 1, 3, 2, 7, 5, 4, 6));
    free(counts);
    graph_free(g);

    return tests_failed;
}
