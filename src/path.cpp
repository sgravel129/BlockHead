/*
#include "../include/path.hpp"
#include "../include/constants.hpp"
#include "../include/graph.hpp"
*/
#include "path.hpp"
#include "constants.hpp"
#include "graph.hpp"

#include <vector>


#define X_MAX SCREEN_WIDTH
#define Y_MAX SCREEN_HEIGHT
// will change these depending on amount of pixels sprite occupies
// #define CLUSTER_WIDTH 120
// #define CLUSTER_HEIGHT 120
#define CLUSTER_SLENGTH 120     // side length of cluster (square)
#define CLUSTER_TLENGTH CLUSTER_SLENGTH / X_STEP
#define CLUSTER_XNUM X_MAX/CLUSTER_SLENGTH
#define CLUSTER_YNUM Y_MAX/CLUSTER_SLENGTH

#define X_STEP 30   // x size of tile
#define Y_STEP 30   // y size of tile
#define MIN_ENTRANCE_LENGTH 3   // minimum tile lenght for entrance
#define TRANSITION_INTERVAL 3   // number of tiles between transitions in single entrance


//TODO
// Implement A* search algorithm
// Implement Destructor
// Integrate with rest of build


// dummy global variable: might want to declare this in separate file, which is run when we load a map
Path_Hierarchy map_hierarchy = Path_Hierarchy(CLUSTER_XNUM * CLUSTER_YNUM);
Abstract_Graph map_graph = Abstract_Graph( CLUSTER_XNUM * CLUSTER_YNUM );
Map game_map = Map();

// Need to add accessor for _tiles in Map class
// accesses map info
PathTile getPathTileFromPoint(const Point& p) {
    //return PathTile(game_map.get_tile(p), map_hierarchy);
    MapTile mt;
    return PathTile(mt, map_hierarchy);    // dummy return for now
}

/////////////////////
// PathTile class implementation
///////////////////////////////////////
//
PathTile::PathTile() {};
PathTile::PathTile(const MapTile& refTile, Path_Hierarchy& parentH) {
    _xPos = refTile.getPos().x;
    _yPos = refTile.getPos().y;
    _traversable = !refTile.getCollision();
    _parentH = &parentH;
    _parentC = findParent();
    
}

Cluster* PathTile::findParent() {
    return _parentH->getClusterAddress(_xPos / CLUSTER_TLENGTH, _yPos / CLUSTER_TLENGTH);
}

// Accessors
int PathTile::get_xPos() { return _xPos; }
int PathTile::get_yPos() { return _yPos; }
bool PathTile::get_traversable() { return _traversable; }
Cluster PathTile::getParentCCopy() { return *_parentC; }

// Memory Accessors
Cluster* PathTile::getParentC() { return _parentC; }
/////////////////////////////////////////////////////////////////////////////
///////////////
// Path_Hierarchy class implementation
////////////////////////////////////////
//
Path_Hierarchy::Path_Hierarchy(const int numClusters) {
    _numTrans = 0;
    _numClusters = numClusters;
}

void Path_Hierarchy::addTransition(const std::pair<PathTile, PathTile>& tilePair) {
    _transitionS.push_back(tilePair);
    _numTrans++;
}

void Path_Hierarchy::buildClusterS() {

    for(int j = 0; j < CLUSTER_YNUM; j++) {
        for(int i = 0; i < CLUSTER_XNUM; i++)
            _clusterS.push_back(Cluster {i,j, NULL});
    }
}

// returns pair of adjacent tiles from cluster pos and tile displacement
std::pair<PathTile, PathTile> Path_Hierarchy::getAdjTiles(const int c1TileX, const int c2TileY, const int k, const bool adjOrientation) {
    Point p1, p2;
    if (adjOrientation) {   // Cluster 1 on left of Cluster 2
        p1.x = k + c1TileX; p1.y = c2TileY - 1;
        p2.x = k + c1TileX; p2.y = c2TileY;
        return std::make_pair(getPathTileFromPoint(p1), getPathTileFromPoint(p2));
    }
    else {                  // Cluster 1 on top of Cluster 2
        p1.x = c1TileX; p1.y = k + c2TileY;
        p2.x = c1TileX + 1; p2.y = k + c2TileY;
        return std::make_pair(getPathTileFromPoint(p1), getPathTileFromPoint(p2));
    }
}


// Accessors
std::vector<std::pair<PathTile, PathTile>> Path_Hierarchy::get_transitionS() { return _transitionS; }
std::pair<PathTile, PathTile> Path_Hierarchy::get_transition(const int transNum) { return _transitionS[transNum]; }
std::vector<Cluster> Path_Hierarchy::get_clusterS() {return _clusterS;}
Cluster Path_Hierarchy::get_cluster(const int x, const int y) {return _clusterS[y*CLUSTER_YNUM + x];}
int Path_Hierarchy::get_numTrans() { return _numTrans; }
int Path_Hierarchy::get_numClusters() { return _numClusters; }

// Private Memory Accessors
Cluster* Path_Hierarchy::getClusterAddress(const int x, const int y) {
    return &(_clusterS[y * CLUSTER_YNUM + x]);
}

bool Path_Hierarchy::getTransitionTileAddresses(const int transNum, PathTile*& t1, PathTile*& t2) {
    if (transNum >= _numTrans)
        return false;
    t1 = &(_transitionS[transNum].first);
    t2 = &(_transitionS[transNum].second);
    return true;
}

//////////////////////////////////////////////////////////////////////////
// gets cluster index in _clusterS vector
int getClusterNum(const Cluster& c) {
    return(c.y*CLUSTER_YNUM + c.x);
}

// Function abstractMap
// divides map into clusters and defines transition points between them
void abstractMap() {

    map_hierarchy.buildClusterS();

    // Adding entrances between clusters for all adjacent clusters
    for(int i = 0; i < CLUSTER_XNUM; i++) {     // checking all vertically adjacent clusters
        for(int j =0; j < CLUSTER_YNUM -1; j++) {
            findTransitions(map_hierarchy.get_cluster(i,j), map_hierarchy.get_cluster(i,j+1));
        }
    }
    
    for(int j = 0; j < CLUSTER_XNUM; j++) {     // checking all laterally adjacent clusters
        for(int i = 0; i < CLUSTER_XNUM - 1; i++) {
            findTransitions(map_hierarchy.get_cluster(i,j), map_hierarchy.get_cluster(i+1,j));
        }
    }
}

// Function findTransitions
// takes two adjacent clusters and finds transition points between them
void findTransitions(const Cluster& c1, const Cluster& c2) {
    int c1TileX = c1.x*CLUSTER_TLENGTH;
    int c2TileY = c2.y*CLUSTER_TLENGTH;

    std::pair<PathTile, PathTile> adjTiles;
    PathTile l1, l2;
    
    bool adjOrientation(c1.x == c2.x);    // True if c1 on top of c2, False if c1 left of c2 

    bool validEnt(false), currEnt(false);
    int entStart(0), entEnd(0);
    

    for(int i = 0; i <  CLUSTER_TLENGTH; i++) {
        adjTiles = getAdjTiles(c1TileX, c2TileY, i, adjOrientation);
        l1 = adjTiles.first;
        l2 = adjTiles.second;
        
        currEnt = (l1.get_traversable() && l2.get_traversable());
        if(currEnt && !validEnt) {  // starting a new entrance
            entStart = i;
            validEnt = true;
        }
        else if((!currEnt && validEnt) || (currEnt && validEnt && i == CLUSTER_TLENGTH-1)) {  // end of entrance
            entEnd = i-1;
            if (entStart == entEnd) {  // if single-tile entrance, define one transition point
                
                map_hierarchy.addTransition(getAdjTiles(c1TileX, c2TileY, entStart, adjOrientation));
            }
            else if(entEnd - entStart < MIN_ENTRANCE_LENGTH) {  // if small entrance, define one transition point in middle
                
                map_hierarchy.addTransition(getAdjTiles(c1TileX, c2TileY, entStart + static_cast<int>((entEnd-entStart)/2), adjOrientation));
            }
            else {      // otherwise define multiple transition points
                for(int k = entStart; k < entEnd+1; k += TRANSITION_INTERVAL)
                    
                    map_hierarchy.addTransition(getAdjTiles(c1TileX, c2TileY, k, adjOrientation));
            }
        }
    }   
}


// Inserts Cluster Transition Tiles into Abstract Graph and sets distances between them as edges
void buildGraph() {
    int cNum1, cNum2;
    Vertex v1, v2;
    PathTile* t1; PathTile* t2;
    std::vector<Vertex*> dummyV;
    std::vector<Edge*> dummyE;
    int numTrans = map_hierarchy.get_numTrans();
    int numClusters = map_hierarchy.get_numClusters();
    for (int i = 0; i < numTrans; i++) {
        if (!map_hierarchy.getTransitionTileAddresses(i, t1, t2))
            break;
        cNum1 = getClusterNum(t1->getParentCCopy());
        cNum2 = getClusterNum(t2->getParentCCopy());
        v1 = { map_graph.getVCNum(cNum1), t1, dummyV, dummyE };
        v2 = { map_graph.getVCNum(cNum2), t2, dummyV, dummyE };
        map_graph.addVertex(v1, cNum1);
        map_graph.addVertex(v2, cNum2);
        map_graph.addEdge(v1.key, v2.key, cNum1, cNum2, 1, INTER);
    }
    int numVC;
    double distance;
    for (int cNum = 0; cNum < numClusters; cNum++) {
        numVC = map_graph.getVCNum(cNum);
        for (int i = 0; i < numVC; i++) {
            for (int j = 0; j < numVC; j++) {
                if (i == j)
                    continue;
                if (!(map_graph.getVertexCopy(i, cNum, v1) && map_graph.getVertexCopy(j, cNum, v2)))
                    break;
                distance = map_graph.searchForDistance(v1, v2, cNum);
                if (distance != HUGE_VAL)
                    map_graph.addEdge(v1.key, v2.key, cNum, cNum, distance, INTRA);
            }
        }
    }
}


void preprocessing(int maxLevel) {
    abstractMap();
    buildGraph();
}