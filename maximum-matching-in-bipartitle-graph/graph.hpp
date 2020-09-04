#pragma once

#include <vector>
#include <unordered_set>
#include <cstdio>

using namespace std;

typedef vector<unordered_set<int>> graph;

// Helper functions

// It is idempotent
void add_edge(int u, int v, graph &adj)
{
    adj[u].insert(v);
}

// Does not complain if edge doesn't exist
void del_edge(int u, int v, graph &adj)
{
    adj[u].erase(v);
}

void print_graph(int n_nodes, const graph &adj)
{
    for (int u = 0; u < n_nodes; u++)
    {
        printf("%d: ", u);
        for(auto v : adj[u])
        {
            printf("%d ", v);
        }
        printf("\n");
    }
}