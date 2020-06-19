#include <stdio.h>
#include <limits.h>
#include "network.cpp"

// Number of vertices in the graph
#define V 303


double minDistance(double dist[], bool sptSet[])
{
    double min = INT_MAX;
    int min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

std::vector<int> dijkstra(double graph[V][V], int src, int dest){
    double dist[V];
    bool sptSet[V];
    int parent[V];

    for (int i = 0; i < V; i++)
    {
        parent[src] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++)
    {
        int u = minDistance(dist, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }

    std::vector<int> path;

    int x = dest;
    while(x != -1){
        path.insert(path.begin(), x);
        x = parent[x];
    }

    return path;
}
