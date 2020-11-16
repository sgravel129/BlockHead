/*
#include "../include/path.hpp"
#include "../include/graph.hpp"
*/
#include "path.hpp"
#include "graph.hpp"

#define X_STEP 30   // x size of tile
#define CLUSTER_SLENGTH 120     // side length of cluster (square)
#define CLUSTER_TLENGTH CLUSTER_SLENGTH / X_STEP

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
bool Abstract_Graph::addEdge(const int key1, const int key2, const int cNum1, const int cNum2, const int d, const edgeType eT, const std::vector<int>& path) {
    Vertex* v1 = getVertexAddress(key1, cNum1);
    Vertex* v2 = getVertexAddress(key2, cNum2);
    if (v1 == NULL || v2 == NULL)
        return false;
    Edge e = {{ v1, v2 }, eT, d, path};
    _edgeL.push_back(e);
    
    v1->adjList.push_back(v2);
    v1->adjEdges.push_back(&_edgeL.back());
    
    v2->adjList.push_back(v1);
    v2->adjEdges.push_back(&_edgeL.back());
    
    _eNum++;
    return true;
}

// Algorithms

// Takes two vertices
double Abstract_Graph::searchForDistance(const Vertex& v1, const Vertex& v2, const int cNum) {

    return 0;
}

std::vector<int> Abstract_Graph::searchForPath(const Point& startP, const Point& goalP) {
    Vertex startV, * startVP, goalV, * goalVP;
    Edge tempEdge;
    
    std::vector<int> path;
    int cNum1 = getClusterNum(startP), cNum2 = getClusterNum(goalP);
    /*
    if start Cluster == goal Cluster
        if both in graph
            return path
        A star algorithm
        if soln
            return path
    */
    if (cNum1 == cNum2) {            // start and goal in same cluster
        if (getVertexCopy(startP, startV) && getVertexCopy(goalP, goalV)) {     // start and goal vertices already in graph
            if (getEdge(startV, goalV, tempEdge))
                return tempEdge.path;
        }
        path = pathFind({ startP.x % CLUSTER_TLENGTH, startP.y % CLUSTER_TLENGTH }, { goalP.x % CLUSTER_TLENGTH, goalP.y % CLUSTER_TLENGTH }, cNum1);
        if (path.size() > 0)
            return path;
    }

    if (!getVertexCopy(startP, startV)) {
        // set flag that we are adding start vertex to graph (for cleanup later)
        // encapsulate vertex into graph
        // connect to all cluster border transitions with A star
        // add edges to graph

        // get startVP
    }
    else
        startVP = getVertexAddress(startV.key, cNum1);
    if (!getVertexCopy(goalP, goalV)) {
        // set flag that we are adding goal vertex to graph (for cleanup later)
        // encapsulate vertex into graph
        // connect to all cluster border transitions with A star
        // add edges to graph
        
        // get goalVP
    }
    else
        goalVP = getVertexAddress(goalV.key, cNum2);


    // Cleanup: delete extra tiles from Path_Hierarchy, and extra vertices and edges from Abstract_Graph

    return path;
}


std::vector<Vertex*> Abstract_Graph::searchForGraphPath(const Vertex* startV, const Vertex* goalV) {
    std::vector<Vertex*> Path;

    /*

    Djikstra's on abstract graph
    return path
    
    */
    return Path;
}

// Takes vector path of Vertices, finds the edges between them, and translates it to int Path info
std::vector<int> Abstract_Graph::graphPathToIntPath(const std::vector<Vertex*>& graphPath) const {
    std::vector<int> intPath, tempPath;
    Vertex* v1, * v2;
    int k;
    for (int i = 0; i < graphPath.size() - 1; i++) {
        v1 = graphPath[i]; v2 = graphPath[static_cast<__int64>(i)+1];
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
bool Abstract_Graph::getVertexCopy(const Point& p, Vertex& v) {
    int cNum = getClusterNum(p);
    Point tempP;
    for (int i = 0; i < _vNums[cNum]; i++) {
        tempP = _vertexS[cNum][i].t->get_mapRelPos();
        if (tempP.x == p.x && tempP.y == p.y) {
            v = _vertexS[cNum][i];
            return true;
        }
    }
    return false;
}


bool Abstract_Graph::getEdge(const Vertex& v1, const Vertex& v2, Edge& e) {
    int tempCNum;
    Vertex tempV;
    int v1CNum = getClusterNum(v1.t->getParentCCopy().clusterPos);
    int v2CNum = getClusterNum(v2.t->getParentCCopy().clusterPos);

    for (auto tempE : v1.adjEdges) {
        tempV = *(tempE->vPair.first);
        tempCNum = getClusterNum(tempV.t->getParentCCopy().clusterPos);
        if (tempV.key == v1.key && tempCNum == v1CNum || tempV.key == v2.key && tempCNum == v2CNum) {
            e = *tempE;
            return true;
        }   
        tempV = *(tempE->vPair.second);
        tempCNum = getClusterNum(tempV.t->getParentCCopy().clusterPos);
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


// Memory Accessor

Vertex* Abstract_Graph::getVertexAddress(const int k, const int cNum) {
    for (int i = k; i < _vNums[cNum]; i++) {
        if (_vertexS[cNum][k].key == k)
            return &_vertexS[cNum][k];
    }
    return NULL;
}
/////////////////////////////////
