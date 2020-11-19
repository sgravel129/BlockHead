

#include "dijkstra.hpp"


using namespace std;


std::pair<int*, int*> Dijkstra(const int src, const int V, int** graph) {
    int* paths;
    

    int* dist = static_cast<int*>(malloc(V * sizeof(int))); // The output array.  dist[i] will hold the shortest 
    // distance from src to i 

    bool* sptSet = static_cast<bool*>(malloc(V * sizeof(bool))); // sptSet[i] will be true if vertex i is included in shortest 
    // path tree or shortest distance from src to i is finalized 

    // Initialize all distances as INFINITE and stpSet[] as false 
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    // Distance of source vertex from itself is always 0 
    dist[src] = 0;

    // Find shortest path for all vertices 
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not 
        // yet processed. u is always equal to src in the first iteration. 
        int u = minDistance(dist, sptSet, V);

        // Mark the picked vertex as processed 
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex. 
        for (int v = 0; v < V; v++)

            // Update dist[v] only if is not in sptSet, there is an edge from 
            // u to v, and total weight of path from src to  v through u is 
            // smaller than current value of dist[v] 
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    // cleanup
    for (int i = 0; i < V; i++) {
        free(graph[i]);
    }
    free(graph); free(sptSet);

    return std::make_pair(dist, paths);

}





int minDistance(int dist[], bool sptSet[], const int V) {
    // Initialize min value 
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// Takes vector path of Vertices, finds the edges between them, and translates it to int Path info
std::vector<int> graphPathToIntPath(const std::vector<Vertex*>& graphPath) {
    std::vector<int> intPath, tempPath;
    Vertex* v1, * v2;
    int k;
    for (int i = 0; i < graphPath.size() - 1; i++) {
        v1 = graphPath[i]; v2 = graphPath[static_cast<__int64>(i) + 1];
        for (int j = 0; j < v1->adjList.size(); j++) {
            if (v1->adjList[j] == v2) {
                tempPath = v1->adjEdges[j]->path;
                intPath.insert(intPath.end(), tempPath.begin(), tempPath.end());
                break;
            }
        }
    }
    return intPath;
}

