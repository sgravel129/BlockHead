#pragma once

#include "path.hpp"


enum edgeType {
    INTER,
    INTRA
};

struct Edge;

typedef struct Vertex {
    Vertex() = default;
    
    Vertex(const Vertex& v) {
        key = v.key;
        cNum = v.cNum;
        t = v.t;
        for (int i = 0; i < v.adjList.size(); i++)
            adjList.push_back(v.adjList[i]);
        for (int i = 0; i < v.adjEdges.size(); i++)
            adjEdges.push_back(v.adjEdges[i]);
    }
    
    Vertex(int k, int c, PathTile* const pt) {
        key = k;
        cNum = c;
        t = pt;
    }
     
    ~Vertex() {
        for (auto ptr : adjList)
            ptr = nullptr;
        for (auto ptr : adjEdges)
            ptr = nullptr;
        t = nullptr;
    }

    int key{};
    int cNum{};
    PathTile* t{};
    std::vector<Vertex*> adjList{};
    std::vector<Edge*> adjEdges{};
} Vertex;

typedef struct Edge {
    Edge() = default;
    Edge(const Edge& e) {
        vPair = std::make_pair(e.vPair.first, e.vPair.second);
        eType = e.eType;
        distance = e.distance;
        for (int i = 0; i < e.path.size(); i++)
            path.push_back(e.path[i]);
    }
    Edge(std::pair<Vertex*, Vertex*> vP, const edgeType eT, double d, std::vector<int> p) {
        eType = eT;
        distance = d;
        for (int i = 0; i < p.size(); i++)
            path.push_back(p[i]);
        vPair = std::make_pair(vP.first, vP.second);
    }
    ~Edge() {
        vPair.first = nullptr;
        vPair.second = nullptr;
    }
    std::pair<Vertex*, Vertex*> vPair{};
    edgeType eType{};
    double distance{};
    std::vector<int> path{};
} Edge;


class Abstract_Graph {
public:
    Abstract_Graph(const int numClusters);
    void addVertex(const Vertex&, const int cNum);
    bool addEdge(const Point&, const Point&, const double, const edgeType, const std::vector<int>&);

    
    double searchForDistance(const Vertex&, const Vertex&, const int cNum);
    std::vector<int> getIntPathFromGraph(const int, const int);
    
    void setWeightedAdj();
    void setNeighborSet();
    void setPaths(std::vector<std::vector<std::vector<int>*>*>* const);
    void setDistances(const std::vector<std::vector<double>>&);

    int keyToGlobalK(const int, const int);
    std::pair<int,int> globalToKey(int);
    
    // Accessors
    bool getVertexCopy(const int k, const int cNum, Vertex& v) const;
    bool getVertexCopy(const Point&, Vertex&) const;  // returns copy of Vertex containing tile of supplied point
    bool getEdge(const Vertex&, const Vertex&, Edge&) const;          // returns copy of Edge between supplied vertices
    bool getEdge(int, int, Edge&);
    int getVCNum(const int) const;        // return number of vertices in supplied cluster
    int getVNum() const;      // return total number of vertices in graph

    double getDistance(const int, const int);
    std::vector<int> getPath(const int, const int);

    

    // Destructor
    ~Abstract_Graph();


    Vertex* getVertexAddress(const Point&) const;    // returns pointer to encapsulated vertex
    std::vector<std::vector<neighbor>*>* get_neighborSet() const;

private:
    std::vector<std::vector<Vertex*>> _vertexS;   // set of vertices, organized by Cluster
    std::vector<Edge*> _edgeL;                    // set of edges
    std::vector<std::vector<double>> _weightedAdj;
    std::vector<std::vector<neighbor>*>* _neighborSet;

    std::vector<int> _vNums;    // number of vertices per cluster
    int _eNum;       // number of edges

    std::vector<std::vector<std::vector<int>*>*>* _paths;
    std::vector<std::vector<double>> _distances;


};

std::vector<int> reverseIntPath(const std::vector<int>&);


