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
Abstract_Graph::Abstract_Graph(const int numClusters) : _weightedAdj(nullptr) {
    _vertexS = std::vector<std::vector<Vertex*>>(numClusters);
    _vNums = std::vector<int>(numClusters, 0);
    _eNum = 0;
};

// Mutators
void Abstract_Graph::addVertex(const Vertex& v, const int cNum) {
    _vertexS[cNum].push_back(new Vertex(v));
    _vNums[cNum]++;
}

// Adds edge to graph connecting to vertices, with supplied distance as weight
bool Abstract_Graph::addEdge(const Point& p1, const Point& p2, const double d, const edgeType eT, const std::vector<int>& path) {
    Vertex* v1 = getVertexAddress(p1);
    Vertex* v2 = getVertexAddress(p2);
    if (v1 == NULL || v2 == NULL)
        return false;
    Edge e = {{ v1, v2 }, eT, d, path};
    _edgeL.push_back(new Edge(e));
    
    v1->adjList.push_back(v2);
    v1->adjEdges.push_back(_edgeL.back());
    
    v2->adjList.push_back(v1);
    v2->adjEdges.push_back(_edgeL.back());
    v1 = nullptr;
    v2 = nullptr;
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
        vTemp->adjList.pop_back();      // we know that the start/goal vertices were the last ones inserted, so just need to pop back
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

// Reverses supplied int vector, returning by address
std::vector<int> reverseIntPath(const std::vector<int>& path) {
    int n = path.size(), temp;
    std::vector<int> newP;
    for (int i = 0; i < n; i++) {
        temp = (path[n - i - 1] + 4) % 8;
        newP.push_back(temp);
    }
    return newP;
}



// set Weighted Adjacency Matrix with edge distances as weights
void Abstract_Graph::setWeightedAdj() {
    int V = getVNum();
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
            v1 = e->vPair.first; v2 = e->vPair.second;
            k1 = keyToGlobalK(v1->key, v1->cNum); k2 = keyToGlobalK(v2->key, v2->cNum);
            adjM[k1][k2] = e->distance;
            adjM[k2][k1] = e->distance;
        }
        delete _weightedAdj;
        _weightedAdj = adjM;
    }
    else {
        Log::error("Failed to allocate memory for path finding");
    }

}


// get Weighted Adjacency Matrix with edge distances as weights
std::vector<std::vector<neighbor>*>* Abstract_Graph::getWeightedAdj() {
    int V = getVNum();
    std::vector<std::vector<neighbor>*>* adjM = new std::vector<std::vector<neighbor>*>(V);
    std::vector<neighbor>* temp = new std::vector<neighbor>;

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (_weightedAdj)
                temp->push_back(neighbor(j, _weightedAdj[i][j]));
        }
        adjM->push_back(temp);
    }
    temp = nullptr;
    return adjM;
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
        if (_vertexS[cNum][k]->key == k) {
            v = *_vertexS[cNum][k];
            return true;
        }
    }
    return false;
}
bool Abstract_Graph::getVertexCopy(const Point& p, Vertex& v) const {
    int cNum = getClusterNum(p);
    Point tempP;
    for (int i = 0; i < _vNums[cNum]; i++) {
        tempP = _vertexS[cNum][i]->t->get_mapRelPos();
        if (tempP.x == p.x && tempP.y == p.y) {
            v = *_vertexS[cNum][i];
            return true;
        }
    }
    return false;
}


bool Abstract_Graph::getEdge(const Vertex& v1, const Vertex& v2, Edge& e) const {
    Point p1, p2;
    if (v1.cNum == v2.cNum && v1.key == v2.key) return false;
    
    for (int i = 0; i < v1.adjList.size(); i++) {        
        
        p1 = v1.adjList[i]->t->get_mapRelPos();
        p2 = v2.t->get_mapRelPos();
        if (p1.x == p2.x && p1.y == p2.y) {
            e = *v1.adjEdges[i];
            if (e.vPair.first->key == v1.key && e.vPair.first->cNum == v1.cNum)
                return true;
            else {
                e.path = reverseIntPath(e.path);
                return true;
            }
                
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
    int cNum = getClusterNum(findParentCluster(p).clusterPos);
    Point tempP;
    for (int i = 0; i < _vNums[cNum]; i++) {
        tempP = _vertexS[cNum][i]->t->get_mapRelPos();
        if (tempP.x == p.x && tempP.y == p.y) {
            return _vertexS[cNum][i];
        }
    }
    return NULL;
}



// Destructor

Abstract_Graph::~Abstract_Graph() {
    delete _weightedAdj;
    _weightedAdj = nullptr;
    
    // All on stack for now... might change later

    /*
    for (int i = 0; i < _vertexS.size(); i++) {
        for (int j = 0; j < _vertexS[i].size(); j++) {
            delete _vertexS[i][j]
        }
    }
    */

}




/////////////////////////////////
