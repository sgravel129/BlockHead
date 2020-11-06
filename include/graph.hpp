#include "path.hpp"
#include <set>

enum edgeType {
    INTER,
    INTRA
};

typedef struct Vertex {
    int key;
    Tile * _tile;
    std::vector<Vertex *> adjList;
    std::vector<Edge *> adjEdges;
} Vertex;

typedef struct Edge {
    std::pair<Vertex *, Vertex *> vPair;
    edgeType eType;
    int distance;
} Edge;


class Map_Path_Graph {
public:
    Map_Path_Graph();
    void addVertex(const Vertex&);
    void addEdge(const Vertex&, const Vertex&, const int, const edgeType);

    int searchForDistance(const Vertex&, const Vertex&, const Cluster&);
    std::vector<Vertex> searchForPath(const Vertex&, const Vertex&);

    // Accessors
    Vertex getVertex(const int, const int, const Cluster&);
    Edge getEdge(const Vertex&, const Vertex&);
    int getAdjInfo(const int&, const int&, const Cluster&);

    // Destructor
    ~Map_Path_Graph();

private:
    std::vector<std::vector<Vertex>> vertexS;   // set of vertices, organized by Cluster
    std::vector<Edge> edgeL;                    // set of edges
    std::vector<std::vector<int>> adjMatrix;     // adjacency matrix
};