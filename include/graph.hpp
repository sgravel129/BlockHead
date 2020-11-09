#include "path.hpp"
#include <set>

enum edgeType {
    INTER,
    INTRA
};

typedef struct Vertex {
    int key;
    Tile * t;
    std::vector<Vertex *> adjList;
    std::vector<Edge *> adjEdges;
} Vertex;

typedef struct Edge {
    std::pair<Vertex *, Vertex *> vPair;
    edgeType eType;
    int distance;
} Edge;


class Abstract_Graph {
public:
    Abstract_Graph(const int numClusters);
    void addVertex(const Vertex&, const int cNum);
    void addEdge(const int, const int, const int, const int, const int, const edgeType);

    double searchForDistance(const Vertex&, const Vertex&, const int cNum);
    std::vector<Vertex> searchForPath(const Vertex&, const Vertex&);

    // Accessors
    
    Vertex getVertexCopy(const int, const int cNum);    // returns copy of Vertex with supplied key in supplied cluster
    Edge getEdge(const Vertex&, const Vertex&);
    int getVCNum(const int);

    // Destructor
    ~Abstract_Graph();

private:
    Vertex* getVertex(const int, const int);    // returns pointer to encapsulated vertex

    std::vector<std::vector<Vertex>> _vertexS;   // set of vertices, organized by Cluster
    std::vector<Edge> _edgeL;                    // set of edges
    std::vector<int> _vNums;    // number of vertices per cluster
    int _eNum;       // number of edges
};