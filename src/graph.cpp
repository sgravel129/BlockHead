//#include "../include/path.hpp"
//#include "../include/graph.hpp"
#include "path.hpp"
#include "graph.hpp"



//////////////////////////////////
// ABSTRACT_GRAPH Implementation
Abstract_Graph::Abstract_Graph(const int numClusters) {
    _vertexS = std::vector<std::vector<Vertex>>(numClusters);
    _vNums = std::vector<int>(numClusters, 0);
};

// Mutators
void Abstract_Graph::addVertex(const Vertex& v, const int cNum) {
    _vertexS[cNum].push_back(v); 
    _vNums[cNum]++;
}

// Adds edge to graph connecting to vertices, with supplied distance as weight
// IMPORTANT THAT POINTERS STORED POINT TO APPROPRIATE ENCAPSULATED ADDRESSES
void Abstract_Graph::addEdge(const int key1, const int key2, const int cNum1, const int cNum2, const int d, const edgeType eT) {
    Vertex* v1 = getVertex(key1, cNum1);
    Vertex* v2 = getVertex(key2, cNum2);
    Edge e = {{ v1, v2 }, eT, d};
    _edgeL.push_back(e);
    
    v1->adjList.push_back(v2);
    v1->adjEdges.push_back(&_edgeL.back());
    
    v2->adjList.push_back(v1);
    v2->adjEdges.push_back(&_edgeL.back());
    
    _eNum++;
}

// Algorithms

// Takes two vertices
double Abstract_Graph::searchForDistance(const Vertex& v1, const Vertex& v2, const int cNum) {

    return 0;
}


std::vector<Vertex> Abstract_Graph::searchForPath(const Vertex& v1, const Vertex& v2) {
    std::vector<Vertex> Path;

    return Path;
}



// Accessors

bool Abstract_Graph::getVertexCopy(const int k, const int cNum, Vertex& v) {
    
    for (int i = k; i < _vNums[cNum]; i++) {
        if (_vertexS[cNum][k].key == k) {
            v = _vertexS[cNum][k];
            return true;
        }
    }
    return false;
}

bool Abstract_Graph::getEdge(const Vertex& v1, const Vertex& v2, Edge& e) {
    int tempCNum;
    Vertex tempV;
    int v1CNum = getClusterNum(*v1.t->parent);
    int v2CNum = getClusterNum(*v2.t->parent);

    for (auto tempE : v1.adjEdges) {
        tempV = *(tempE->vPair.first);
        tempCNum = getClusterNum(*tempV.t->parent);
        if (tempV.key == v1.key && tempCNum == v1CNum || tempV.key == v2.key && tempCNum == v2CNum) {
            e = *tempE;
            return true;
        }   
        tempV = *(tempE->vPair.second);
        tempCNum = getClusterNum(*tempV.t->parent);
        if (tempV.key == v1.key && tempCNum == v1CNum || tempV.key == v2.key && tempCNum == v2CNum) {
            e = *tempE;
            return true;
        }   
    }
    return false;
}

int Abstract_Graph::getVCNum(const int cNum) {
    return _vNums[cNum];
}


// Private Accessor

Vertex* Abstract_Graph::getVertex(const int k, const int cNum) {
    for (int i = k; i < _vNums[cNum]; i++) {
        if (_vertexS[cNum][k].key == k)
            return &_vertexS[cNum][k];
    }
    return NULL;
}
/////////////////////////////////
