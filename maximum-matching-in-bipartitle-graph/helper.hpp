#pragma once

#include "graph.hpp"
#include <vector>
#include <queue>

using namespace std;

/*
 * Helpers for getting maximum matching by
 * augmenting one path at a time
 */

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


/*
 * Helpers for getting maximum matching by
 * augmenting maximal set of disjoint paths at a
 * time
 */

void bfs(int n_nodes, int source, int destination, const graph &adj, graph &bfs_dag)
{
    vector<bool> explored(n_nodes, false);
    queue<int> q;
    q.push(source);

    while(!q.empty())
    {
        int u = q.front();
        for(auto v : adj[u])
        {
            if(explored[v] == false)
            {
                add_edge(u, v, bfs_dag);
                q.push(v);
            }
        }
        q.pop();
        explored[u] = true;
    }
}

void bfs_wrapper(int n_nodes, int source, int destination, const graph &adj, graph &bfs_dag)
{
    bfs_dag.resize(n_nodes);
    bfs(n_nodes, source, destination, adj, bfs_dag);
}

void dfs_on_dag(int u, int destination, const graph &dag, vector<bool> &visited, vector<int> &path)
{
    if (u == destination)
    {
        path.push_back(u);
        return;
    }
    if (visited[u])
    {
        return;
    }
    visited[u] = true;

    for (auto v : dag[u])
    {
        if (!visited[v])
        {
            dfs_on_dag(v, destination, dag, visited, path);
            if (!path.empty())
            {
                path.push_back(u);
                return;
            }
        }
    }
}

// Get maximal set of (reverse) disjoint paths from `source` to `destination`
// Note: Two paths with common source and destination are disjoint
// if they don't have common edges
void get_disjoint_paths(int n_nodes, int source, int destination, const graph &adj, vector<vector<int>>& disjoint_paths)
{
    // bfs tree from `source`, but with edges oriented in reverse direction
    graph bfs_dag;
    bfs_wrapper(n_nodes, source, destination, adj, bfs_dag);

    vector<bool> visited(n_nodes, false);
    for(auto v : bfs_dag[source])
    {
        vector<int> path;
        dfs_on_dag(v, destination, adj, visited, path);
        if(!path.empty())
        {
            path.push_back(source);
            disjoint_paths.push_back(path);
        }
    }
}