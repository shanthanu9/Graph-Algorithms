#include <cstdio>
#include <cstdlib>
#include <ctime>

// Outputs a random bipartite graph
// NOTE: Some edges maybe repeated

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("expected args: ./executable <number-of-nodes-left>"
               " <number-of-nodes-right> <number-of-edges>");
        return 1;
    }

    // srand(time(0)); // random on each run

    // Input
    int a, b, m;
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    m = atoi(argv[3]);

    // Output nodes and edges
    printf("%d %d\n", a+b, m); // first output number of nodes and edges
    for(int i = 0; i < m; i++) // output 'm' edges
    {
        int u = rand()%a+1;
        int v = a+rand()%b+1;
        printf("%d %d\n", u, v);
    }

    return 0;
}