### Input

The first two lines are the number of nodes and edges in the graph, `n` and `m`
respectively.

The next `m` lines contains two numbers each denoting the edges of the graph.
Note that the nodes have to be one-indexed. The edges should follow the below
constraint:

If the bipartite graph can be partitioned into two set `A` and `B` (imagine `A`
on the left side and `B` on the right), then for each edge in input, the first
node should be from `A` and second node should be from `B`.

For eg, following is a valid input:
```
5 4
1 3
1 4
2 4
2 5
```

Here, nodes 1, 2 are in `A` and 3, 4, 5 are in `B`.

### Output

The first line contains the size of maximum matching, say `s` followed by `s`
lines of edges in a maximum matching.

For eg, the output for the above example will be
```
2
1 3
2 5
```