#include "graph.hpp"
#include "log.hpp"
#define TLENGTH 75   // x size of tile
#define CLUSTER_TLENGTH 8


//////////////////////////////////
// ABSTRACT_GRAPH Implementation
Abstract_Graph::Abstract_Graph(const int numClusters) : _weightedAdj({}), _neighborSet(nullptr), _paths(nullptr), _distances({}) {
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



//////////////
// Algorithms

// Takes two vertices
double Abstract_Graph::searchForDistance(const Vertex& v1, const Vertex& v2, const int cNum) {

    return 0;
}




std::vector<int> Abstract_Graph::getIntPathFromGraph(const int startGlobalK, const int goalGlobalK) {
    std::vector<int> vertexPath;
    std::vector<int> dirPath;
    Edge e;

    vertexPath = getPath(startGlobalK, goalGlobalK);
    
    for (int i = 0; i < vertexPath.size() - 1; i++) {
        if (!getEdge(vertexPath[i], vertexPath[i+1], e)) break;
        dirPath.insert(dirPath.end(), e.path.begin(), e.path.end());
    }
    

    return dirPath;
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
    _weightedAdj.resize(V);
    for (int i = 0; i < V; i++)
        _weightedAdj[i].resize(V, 0);
    

    Vertex* v1, * v2;
    int k1, k2;
    for (auto e : _edgeL) {
        v1 = e->vPair.first; v2 = e->vPair.second;
        k1 = keyToGlobalK(v1->key, v1->cNum); k2 = keyToGlobalK(v2->key, v2->cNum);
        _weightedAdj[k1][k2] = e->distance;
        _weightedAdj[k2][k1] = e->distance;
        }

}


// TO DO: ALLOCATE MORE SPACE TO ACCOUNT FOR ADDING START AND GOAL
// get Weighted Adjacency Matrix with edge distances as weights
void Abstract_Graph::setNeighborSet() {
    int V = getVNum();
    delete _neighborSet;
    std::vector<std::vector<neighbor>*>* adjM = new std::vector<std::vector<neighbor>*>(V);
   // std::vector<neighbor>* temp = new std::vector<neighbor>;

    for (int i = 0; i < V; i++) {
        adjM->at(i) = new std::vector<neighbor>;
        for (int j = 0; j < V; j++) {
            if (_weightedAdj[i][j])
                adjM->at(i)->push_back(neighbor(j, _weightedAdj[i][j]));
        }
        //adjM->push_back(temp);
    }
  //  temp = nullptr;
    _neighborSet = adjM;
}


void Abstract_Graph::setPaths(std::vector<std::vector<std::vector<int>*>*>* const paths) { _paths = paths; }
void Abstract_Graph::setDistances(const std::vector<std::vector<double>>& distances) { _distances = distances; }








int Abstract_Graph::keyToGlobalK(const int key, const int cNum) {
    int globalKey = 0;
    for (int i = 0; i < cNum; i++) {
        globalKey += _vNums[i];
    }
    return globalKey + key;
}

std::pair<int, int> Abstract_Graph::globalToKey(int gKey) {
    int cNum = -1;
    while (gKey >= 0) {
        gKey -= _vNums[++cNum];
    }
    return std::make_pair(_vNums[cNum]+gKey, cNum);
}


// Accessors

bool Abstract_Graph::getVertexCopy(const int k, const int cNum, Vertex& v) const {
    try {
        v = *_vertexS.at(cNum).at(k);
        return true;
    }
    catch (const std::out_of_range& e) { 
        std::cout << std::endl << std::endl << "LMAO U RETARD" << std::endl << std::endl; }
    return false;
    /*
    for (int i = k; i < _vNums[cNum]; i++) {
        if (_vertexS[cNum][i]->key == k) {
            v = *_vertexS[cNum][i];
            return true;
        }
    }
    return false;
    */
}
bool Abstract_Graph::getVertexCopy(const Point& p, Vertex& v) const {
    int cNum = getClusterNum(findParentCluster(p).clusterPos);
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


bool Abstract_Graph::getEdge(int gKey1, int gKey2, Edge& e) {
    Point p1, p2;
    if (gKey1 == gKey2) return false;
    std::pair<int, int> v1Key = globalToKey(gKey1), v2Key = globalToKey(gKey2);
    Vertex v1, v2;
    if (!getVertexCopy(v1Key.first, v1Key.second, v1) || !getVertexCopy(v2Key.first, v2Key.second, v2)) return false;
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


double Abstract_Graph::getDistance(const int x, const int y) { return _distances[x][y]; }
std::vector<int> Abstract_Graph::getPath(const int x, const int y) { return *_paths->at(x)->at(y); }

// Memory Accessor

Vertex* Abstract_Graph::getVertexAddress(const Point& p) const {
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

std::vector<std::vector<neighbor>*>* Abstract_Graph::get_neighborSet() const { return _neighborSet; };


// Destructor

Abstract_Graph::~Abstract_Graph() {

    for (int i = 0; i < _vertexS.size(); i++) {
        for (int j = 0; j < _vertexS[i].size(); j++) {
            delete _vertexS[i][j];
        }
    }
    for (int i = 0; i < _edgeL.size(); i++)
        delete _edgeL[i];

    if (_neighborSet) {
        for (int i = 0; i < _neighborSet->size(); i++) {
            delete _neighborSet->at(i);
            _neighborSet->at(i) = nullptr;
        }
        delete _neighborSet;
    }
    if (_paths) {
        for (int i = 0; i < _paths->size(); i++) {
            for (int j = 0; j < _paths->at(i)->size(); j++) {
                delete _paths->at(i)->at(j);
                _paths->at(i)->at(j) = nullptr;
            }
            delete _paths->at(i);
            _paths->at(i) = nullptr;
        }
        delete _paths;
        _paths = nullptr;
    }

}




/////////////////////////////////
