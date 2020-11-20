/*
#include "../include/path.hpp"
#include "../include/graph.hpp"
*/
//#include "path.cpp"
#include "graph.hpp"
#include "log.hpp"
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
bool Abstract_Graph::addEdge(const Point& p1, const Point& p2, const int d, const edgeType eT, const std::vector<int>& path) {
    Vertex* v1 = getVertexAddress(p1);
    Vertex* v2 = getVertexAddress(p2);
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

// DELETE Start/Goal Vertex
void Abstract_Graph::deleteStartAndGoal(const Vertex* v, const int cNum) {
    Vertex* vTemp;
    // first iterate through neighbors, and delete all edge information in neighbour vertices
    int numNeighbours = v->adjList.size();
    for (int i = 0; i < numNeighbours; i++) {
        vTemp = getVertexAddress(v->adjList[i]->t->get_mapRelPos());
        vTemp->adjList.pop_back();      // we know that the start vertex was the last one inserted, so just need to pop back
        vTemp->adjEdges.pop_back();
    }
    _vertexS[cNum].pop_back();
    _edgeL.pop_back();
    _vNums[cNum]--;
    _eNum--;
}


//////////////
// Algorithms

// Takes two vertices
double Abstract_Graph::searchForDistance(const Vertex& v1, const Vertex& v2, const int cNum) {

    return 0;
}




std::vector<Vertex*> Abstract_Graph::searchForGraphPath(const Vertex* startV, const Vertex* goalV) {
    std::vector<Vertex*> Path;

    /*

    Djikstra's on abstract graph
    return path
    
    */
    return Path;
}



// get Weighted Adjacency Matrix with edge distances as weights
int** Abstract_Graph::getWeightedAdj(const int V) {
    
    int** adjM = new intArrayPtr[V];

    for (int i = 0; i < V; i++)
        adjM[i] = new int[V];
    
    if (adjM) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++)
                adjM[i][j] = 0;
        }

        Vertex* v1, * v2;
        int k1, k2;
        for (auto e : _edgeL) {
            v1 = e.vPair.first; v2 = e.vPair.second;
            k1 = keyToGlobalK(v1->key, v1->cNum); k2 = keyToGlobalK(v2->key, v2->cNum);
            adjM[k1][k2] = e.distance;
            adjM[k2][k1] = e.distance;
        }
        return adjM;
    }
    else {
        Log::error("Failed to allocate memory for path finding");
        return NULL;
    }

}

int Abstract_Graph::keyToGlobalK(const int key, const int cNum) {
    int globalKey = 0;
    for (int i = 0; i < cNum; i++) {
        globalKey += _vNums[i];
    }
    return globalKey + key;
}


// Accessors

bool Abstract_Graph::getVertexCopy(const int k, const int cNum, Vertex& v) const {
    
    for (int i = k; i < _vNums[cNum]; i++) {
        if (_vertexS[cNum][k].key == k) {
            v = _vertexS[cNum][k];
            return true;
        }
    }
    return false;
}
bool Abstract_Graph::getVertexCopy(const Point& p, Vertex& v) const {
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


bool Abstract_Graph::getEdge(const Vertex& v1, const Vertex& v2, Edge& e) const {
    Point p1, p2;
    for (int i = 0; i < v1.adjList.size(); i++) {
        p1 = v1.adjList[i]->t->get_mapRelPos();
        p2 = v2.t->get_mapRelPos();
        if (p1.x == p2.x && p1.y == p2.y) {
            e = *v1.adjEdges[i];
            return true;
        }
    }
    return false;
}

int Abstract_Graph::getVCNum(const int cNum) const {
    return _vNums[cNum];
}

int Abstract_Graph::getVNum() const {
    int sum(0);
    for (int i = 0; i < _vNums.size(); i++)
        sum += _vNums[i];
    return sum;
}

// Memory Accessor

Vertex* Abstract_Graph::getVertexAddress(const Point& p) {
    int cNum = getClusterNum(p);
    Point tempP;
    for (int i = 0; i < _vNums[cNum]; i++) {
        tempP = _vertexS[cNum][i].t->get_mapRelPos();
        if (tempP.x == p.x && tempP.y == p.y) {
            return &_vertexS[cNum][i];
        }
    }
    return NULL;
}
/////////////////////////////////
