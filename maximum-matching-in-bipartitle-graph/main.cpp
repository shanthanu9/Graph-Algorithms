#include <stdio.h>
#include <vector>
#include <set>

using namespace std;

typedef vector<set<int>> graph;

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

void dfs(int u, const graph &adj, vector<int> &parent)
{
    for (auto v : adj[u])
    {
        if (parent[v] == -1)
        {
            parent[v] = u;
            dfs(v, adj, parent);
        }
    }
}

void dfs_wrapper(int n_nodes, int source, const graph &adj, vector<int> &parent)
{
    parent.resize(n_nodes, -1);
    dfs(source, adj, parent);
}

// Returns a reverse path from `destination` to `source` if reachable.
// Else returns an empty list.
void get_path(int n_nodes, int source, int destination, const graph &adj, vector<int> &path)
{
    vector<int> parent{};
    dfs_wrapper(n_nodes, source, adj, parent);

    path.clear();
    if (parent[destination] != -1)
    {
        int u = destination;
        while(parent[u] != -1)
        {
            path.push_back(u);
            u = parent[u];
        }
        path.push_back(u);
    }
}

void augment_matching(const vector<int> &augmenting_path, graph &adj, set<int> &matching)
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
    set<int> matching; // keeps track of right end node of the matching
    set<int> right_side_nodes; // keep track of nodes on the right side

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

    // Find maximum matching
    while(true)
    {
        // Get a path from source to sink
        // If a path exists, then that is also the
        // augmenting path
        vector<int> augmenting_path;
        get_path(n+2, 0, n+1, adj, augmenting_path);

        // Augment matching if augmenting path exists
        if (!augmenting_path.empty())
        {
            // Update source and sink node edges for next iteration
            int n_path = augmenting_path.size();
            del_edge(augmenting_path[1], augmenting_path[0], adj);
            del_edge(augmenting_path[n_path-1], augmenting_path[n_path-2], adj);

            // Remove source and sink nodes from augmenting path
            augmenting_path.erase(augmenting_path.begin());
            augmenting_path.erase(augmenting_path.end()-1);

            augment_matching(augmenting_path, adj, matching);
        }
        else
        {
            break;
        }
    }

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

    // Output maximum matching
    printf("%d\n", matching.size()); // Output size

    // Output edges
    for (auto v : matching)
    {
        int u = *(adj[v].begin());
        printf("%d %d\n", u, v);
    }

    return 0;
}