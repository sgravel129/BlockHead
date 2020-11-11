#pragma once

//#include "../include/path.hpp"
#include "path.hpp"

enum edgeType {
    INTER,
    INTRA
};

struct Edge;

typedef struct Vertex {
    int key;
    PathTile * t;
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
    bool addEdge(const int, const int, const int, const int, const int, const edgeType);

    double searchForDistance(consst Vertex&, const Vertex&, const int cNum);
    std::vector<Vertex> searchForPath(const Vertex&, const Vertex&);

    // Accessors
    
    bool getVertexCopy(const int, const int cNum, Vertex&);    // returns copy of Vertex with supplied key in supplied cluster
    bool getEdge(const Vertex&, const Vertex&, Edge&);          // returns copy of Edge between supplied vertices
    int getVCNum(const int);        // return number of vertices in supplied cluster

    // Destructor
    ~Abstract_Graph();

private:
    Vertex* getVertexAddress(const int, const int);    // returns pointer to encapsulated vertex

    std::vector<std::vector<Vertex>> _vertexS;   // set of vertices, organized by Cluster
    std::vector<Edge> _edgeL;                    // set of edges
    std::vector<int> _vNums;    // number of vertices per cluster
    int _eNum;       // number of edges
};