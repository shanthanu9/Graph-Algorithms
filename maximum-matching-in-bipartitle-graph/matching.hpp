#pragma once

#include "graph.hpp"
#include "helper.hpp"

#include <vector>
#include <unordered_set>
#include <cmath>

using namespace std;

/*
 * Given an augmenting path, augment the matching.
 */
void augment_matching(const vector<int> &augmenting_path, graph &adj, unordered_set<int> &matching)
{
    for (int i = augmenting_path.size()-1; i > 0; i--)
    {
        // Reverse edges on the augmenting path
        int u = augmenting_path[i];
        int v = augmenting_path[i-1];
        del_edge(u, v, adj);
        add_edge(v, u, adj);
    }
    // Insert the newly added node on the right side in the matching
    matching.insert(augmenting_path[0]);
}

/*
 * Remove auxillary source and sink nodes from the augmenting path
 */
void cleanup_augmenting_path(graph &adj, vector<int> &augmenting_path)
{
    // Update source and sink node edges for next iteration
    int n_path = augmenting_path.size();
    del_edge(augmenting_path[1], augmenting_path[0], adj);
    del_edge(augmenting_path[n_path-1], augmenting_path[n_path-2], adj);

    // Remove source and sink nodes from augmenting path
    augmenting_path.erase(augmenting_path.begin());
    augmenting_path.erase(augmenting_path.end()-1);
}

/*
 * Find maximum matching by augmenting one path at a time
 */
void find_mm_by_single_path_augment(int n, graph &adj, unordered_set<int> &matching, int max_iterations = -1)
{
    if (max_iterations == -1)
    {
        max_iterations = n/2;
    }

    // Find maximum matching
    for(int i = 0; i < max_iterations; i++)
    {
        // Get a path from source to sink
        // If a path exists, then that is also the
        // augmenting path
        vector<int> augmenting_path;
        get_path(n+2, 0, n+1, adj, augmenting_path);

        if (augmenting_path.empty()) // maximum matching found
        {
            break;
        }

        cleanup_augmenting_path(adj, augmenting_path);
        augment_matching(augmenting_path, adj, matching);
    }
}

/*
 * Find maximum matching by augmenting set of
 * maximal disjoint paths at a time
 */
void find_mm_by_disjoint_paths_augment(int n, graph &adj, unordered_set<int> &matching, int max_iterations = -1)
{
    if (max_iterations == -1)
    {
        max_iterations = n/2;
    }

    // Find maximum matching
    for(int i = 0; i < max_iterations; i++)
    {
        vector<vector<int>> disjoint_aug_paths;
        get_disjoint_paths(n+2, 0, n+1, adj, disjoint_aug_paths);

        if(disjoint_aug_paths.empty()) // maximum matching found
        {
            break;
        }

        for(auto augmenting_path : disjoint_aug_paths)
        {
            cleanup_augmenting_path(adj, augmenting_path);
            augment_matching(augmenting_path, adj, matching);
        }
    }
}

/*
 * Naive method:
 *
 * At each iteration, augment matching by a single path
 *
 * Time complexity: O(m*n)
 */
void find_mm_naive(int n, graph &adj, unordered_set<int>& matching)
{
    find_mm_by_single_path_augment(n, adj, matching);
}

/*
 * Hopcraft Karp mathod:
 *
 * For first `sqrt(n)-1` steps, augment matching
 * by using multiple same length disjoint paths.
 * In the next (atmost) `sqrt(n)` steps, augment matching
 * one path at a time
 *
 * Time complexity: O(m*sqrt(n))
 */
void find_mm_hopcraft_karp(int n, graph &adj, unordered_set<int>& matching)
{
    find_mm_by_disjoint_paths_augment(n, adj, matching, ceil(sqrt(n)-1));
    find_mm_by_single_path_augment(n, adj, matching);
}

bool verify_maximum_matching(int n, const graph &adj)
{
    vector<int> augmenting_path;
    get_path(n+2, 0, n+1, adj, augmenting_path);
    return augmenting_path.empty();
}