#pragma once

//#include "../include/path.hpp"
#include "path.hpp"

enum edgeType {
    INTER,
    INTRA
};

struct Edge;

typedef struct Vertex {
    int key{};
    int cNum{};
    PathTile* t{};
    std::vector<Vertex*> adjList{};
    std::vector<Edge*> adjEdges{};
} Vertex;

typedef struct Edge {
    std::pair<Vertex*, Vertex*> vPair{};
    edgeType eType{};
    int distance{};
    std::vector<int> path{};
} Edge;


class Abstract_Graph {
public:
    Abstract_Graph(const int numClusters);
    void addVertex(const Vertex&, const int cNum);
    bool addEdge(const Point&, const Point&, const int, const edgeType, const std::vector<int>&);

    
    double searchForDistance(const Vertex&, const Vertex&, const int cNum);
    std::vector<int> searchForPath(const Point&, const Point&);
    std::vector<Vertex*> searchForGraphPath(const Vertex*, const Vertex*);
    int* Dijkstra(const int);
    int** getWeightedAdj(const int);
    std::vector<int> graphPathToIntPath(const std::vector<Vertex*>&) const ;

    
    // Accessors
    bool getVertexCopy(const int k, const int cNum, Vertex& v) const;
    bool getVertexCopy(const Point&, Vertex&) const;  // returns copy of Vertex containing tile of supplied point
    bool getEdge(const Vertex&, const Vertex&, Edge&) const;          // returns copy of Edge between supplied vertices
    int getVCNum(const int) const;        // return number of vertices in supplied cluster
    int getVNum() const;      // return total number of vertices in graph

    // Destructor
    ~Abstract_Graph();

private:
    Vertex* getVertexAddress(const Point&);    // returns pointer to encapsulated vertex
    void deleteStartAndGoal(const Vertex*, const int);

    std::vector<std::vector<Vertex>> _vertexS;   // set of vertices, organized by Cluster
    std::vector<Edge> _edgeL;                    // set of edges
    std::vector<int> _vNums;    // number of vertices per cluster
    int _eNum;       // number of edges


};

int minDistance(int dist[], bool sptSet[]);
int keyToGlobalK(const int, const int);
