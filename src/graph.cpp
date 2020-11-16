/*
#include "../include/path.hpp"
#include "../include/graph.hpp"
*/
#include "path.cpp"
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
    delete vTemp;
}


//////////////
// Algorithms

// Takes two vertices
double Abstract_Graph::searchForDistance(const Vertex& v1, const Vertex& v2, const int cNum) {

    return 0;
}

std::vector<int> Abstract_Graph::searchForPath(const Point& startP, const Point& goalP) {
    Vertex startV, * startVP, goalV, * goalVP;
    Edge tempEdge;
    PathTile* tempT;
    bool addStart(false), addGoal(false);
    Point p1, p2;
    std::vector<int> intPath;
    std::vector<Vertex*> graphPath;
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
        intPath = pathFind({ startP.x % CLUSTER_TLENGTH, startP.y % CLUSTER_TLENGTH }, { goalP.x % CLUSTER_TLENGTH, goalP.y % CLUSTER_TLENGTH }, cNum1);
        if (intPath.size() > 0)
            return intPath;
    }

    if (!getVertexCopy(startP, startV)) {
        // set flag that we are adding start vertex to graph (for cleanup later)
        addStart = true;
        // encapsulate vertex into graph
        tempT = new PathTile(*getMapTileFromPoint(startP), map_hierarchy);
        map_hierarchy.addStart(tempT);
        // connect to all cluster border transitions with A star
        p1 = { startP.x % CLUSTER_TLENGTH, startP.y % CLUSTER_TLENGTH };
        map_hierarchy.getStartAddress(tempT);
        addVertex({ getVCNum(cNum1), cNum1, tempT, {}, {} }, cNum1);
        // add edges to graph
        for (int i = 0; i < _vNums[cNum1] - 1; i++) {
            p2 = _vertexS[cNum1][i].t->get_clusterRelPos();
            intPath = pathFind(p1, p2, cNum1);
            addEdge(p1, p2, pathToDistance(intPath), INTRA, intPath);
        }
    }
    startVP = getVertexAddress(startP);

    if (!getVertexCopy(goalP, goalV)) {
        // set flag that we are adding goal vertex to graph (for cleanup later)
        addGoal = true;
        // encapsulate vertex into graph
        tempT = new PathTile(*getMapTileFromPoint(goalP), map_hierarchy);
        map_hierarchy.addGoal(tempT);
        // connect to all cluster border transitions with A star
        p1 = { goalP.x % CLUSTER_TLENGTH, goalP.y % CLUSTER_TLENGTH };
        map_hierarchy.getGoalAddress(tempT);
        addVertex({ getVCNum(cNum2), cNum2, tempT, {}, {} }, cNum2);
        // add edges to graph
        for (int i = 0; i < _vNums[cNum2] - 1; i++) {
            p2 = _vertexS[cNum2][i].t->get_clusterRelPos();
            intPath = pathFind(p1, p2, cNum2);
            addEdge(p1, p2, pathToDistance(intPath), INTRA, intPath);
        }
    }
    goalVP = getVertexAddress(goalP);

    // Start and Goal in abstract Graph: can now proceed with graph search
    graphPath = searchForGraphPath(startVP, goalVP);

    // Cleanup: delete extra tiles from Path_Hierarchy, and extra vertices and edges from Abstract_Graph
    map_hierarchy.deleteStartAndGoal();
    if (addStart)
        deleteStartAndGoal(startVP, cNum1);
    if (addGoal)
        deleteStartAndGoal(goalVP, cNum2);

    return graphPathToIntPath(graphPath);
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

int Abstract_Graph::getVCNum(const int cNum) {
    return _vNums[cNum];
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
