

#include "dijkstra.hpp"

const weight_t max_weight = std::numeric_limits<double>::infinity();

using namespace std;

// Takes vector path of Vertices, finds the edges between them, and translates it to int Path info
std::vector<int> graphPathToIntPath(const std::vector<Vertex*>& graphPath) {
    std::vector<int> intPath, tempPath;
    Vertex* v1, * v2;
    int k;
    for (int i = 0; i < graphPath.size() - 1; i++) {
        v1 = graphPath[i]; v2 = graphPath[static_cast<int64_t>(i) + 1];
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



void DijkstraComputePaths(vertex_t source,
    std::vector<std::vector<neighbor>*>* const adjacency_list,
    std::vector<weight_t>* const min_distance,
    std::vector<vertex_t>& previous)
{
    int n = adjacency_list->size();
    min_distance->clear();
    min_distance->resize(n, max_weight);
    min_distance->at(source) = 0;
    previous.clear();
    previous.resize(n, -1);
    std::set<std::pair<weight_t, vertex_t> > vertex_queue;
    vertex_queue.insert(std::make_pair(min_distance->at(source), source));

    while (!vertex_queue.empty())
    {
        weight_t dist = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
        const std::vector<neighbor>& neighbors = *adjacency_list->at(u);
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
            neighbor_iter != neighbors.end();
            neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = dist + weight;
            if (distance_through_u < min_distance->at(v)) {
                vertex_queue.erase(std::make_pair(min_distance->at(v), v));

                min_distance->at(v) = distance_through_u;
                previous[v] = u;
                vertex_queue.insert(std::make_pair(min_distance->at(v), v));

            }

        }
    }
}


std::vector<vertex_t>* DijkstraGetShortestPathTo(
    vertex_t vertex, const std::vector<vertex_t>& previous)
{
    std::vector<vertex_t>* path = new std::vector<vertex_t>;
    for (; vertex != -1; vertex = previous[vertex])
        path->insert(path->begin(), vertex);

    return path;
}