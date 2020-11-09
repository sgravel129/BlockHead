#include "path.hpp"
#include "graph.hpp"







//////////////////////////////////
// ABSTRACT_GRAPH Implementation
Abstract_Graph::Abstract_Graph(const int numClusters) {
    _vertexS = std::vector<std::vector<Cluster>>(numClusters);
    _vNums = std::vector<int>(numClusters, 0);
};

// Mutators
void Abstract_Graph::addVertex(const Vertex& v, const int cNum) {
    _vertexS[cNum].push_back(v); 
    _vNum[cNum]++;
}

// Adds edge to graph connecting to vertices, with supplied distance as weight
// IMPORTANT THAT POINTERS STORED POINT TO APPROPRIATE ENCAPSULATED ADDRESSES
void Abstract_Graph::addEdge(const int key1, const int key2, const int cNum1, const int cNum2, const int d, const edgeType eT) {
    Vertex* v1 = getVertex(key1, cNum1);
    Vertex* v2 = getVertex(key2, cNum2);

    _edgeL.emplace_back({v1, v2}, eT, d);
    
    v1->adjList.push_back(v2);
    v1->adjEdges.push_back(&_edgeL.back());
    
    v2->adjList.push_back(v1);
    v2->adjEdges.push_back(&_edgeL.back());
    
    _eNum++;
}

// Algorithms

// Takes two vertices
int Abstract_Graph::searchForDistance(const Vertex& v1, const Vertex& v2, const int cNum) {

    return 0;
}


std::vector<Vertex> Abstract_Graph::searchForPath(const Vertex& v1, const Vertex& v2) {
    std::vector<Vertex> Path;

    return Path;
}



// Accessors

Vertex Abstract_Graph::getVertexCopy(const int k, const int cNum) {
    
    for (int i = k; i < _vNum; i++) {
        if (_vertexS[cNum][k].key == k)
            return _vertexS[cNum][k];
    }
    return NULL;
}

Edge Abstract_Graph::getEdge(const Vertex& v1, const Vertex& v2) {
    for (auto e : v1.adjEdges) {
        if (e->vPair.first == v2 || e->vPair.second == v2)
            return e;
    }
    return NULL;
}

int Abstract_Graph::getVCNum(const int cNum) {
    return _vNums[cNum];
}


// Private Accessor

Vertex* Abstract_Graph::getVertex(const int k, const int cNum) {
    for (int i = k; i < _vNum; i++) {
        if (_vertexS[cNum][k].key == k)
            return &_vertexS[cNum][k];
    }
    return NULL;
}
/////////////////////////////////
