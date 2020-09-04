#include "graph.hpp"
#include "matching.hpp"

#include <cstdio>
#include <vector>
#include <chrono>
#include <iostream>
#include <unordered_set>

using namespace std;
using namespace std::chrono;

/*
 * Non-matched edges in bipartite graph are oriented
 * from left to right.
 * Matched edges are oriented from right to left.
 *
 * 0 and n+1 are special source and sink nodes of left and
 * right side respectively.
 */
int main()
{
    // Graph parameters
    int n, m; // number of nodes, edges
    graph adj; // adjacency list representation
    unordered_set<int> matching; // keeps track of right end node of the matching
    unordered_set<int> right_side_nodes; // keep track of nodes on the right side

    scanf("%d %d", &n, &m);
    adj.resize(n+2);

    /*
     * Suppose the bipartite graph is partitioned into
     * two sets, A and B (A is on left side, and B on right).
     * The input edges (u, v) should be given such that
     * u is in A and v is in B.
     *
     * The graph nodes should be one-indexed.
     */
    for (int i = 0; i < m; i++)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        add_edge(u, v, adj);
        right_side_nodes.insert(v);
    }

    // Add special source and sink nodes
    for (int u = 1; u <= n; u++)
    {
        if (right_side_nodes.count(u) == 0)
        {
            add_edge(0, u, adj);
        }
        else
        {
            add_edge(u, n+1, adj);
        }
    }

    auto start = high_resolution_clock::now();

    find_mm_naive(n, adj, matching);
    if(verify_maximum_matching(n, adj))
    {
        printf("Correct answer!\n");
    }

    auto stop = high_resolution_clock::now();

    // Delete special source and sink nodes
    for (int u = 1; u <= n; u++)
    {
        if (right_side_nodes.count(u) == 0)
        {
            del_edge(0, u, adj);
        }
        else
        {
            del_edge(u, n+1, adj);
        }
    }

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Time taken: " << duration.count() << "ms" << endl;

    // Output maximum matching
    printf("Maximum matching size: %d\n", matching.size()); // Output size

    // Output edges
    // for (auto v : matching)
    // {
    //     int u = *(adj[v].begin());
    //     printf("%d %d\n", u, v);
    // }

    return 0;
}