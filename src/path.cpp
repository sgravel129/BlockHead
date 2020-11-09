#include "path.hpp"
#include "constants.hpp"
#include "graph.hpp"
#include <vector>

// dummy global variable: might want to declare this in separate file, which is run when we load a map
Path_Hierarchy map_hierarchy = Path_Hierarchy(CLUSTER_XNUM * CLUSTER_YNUM);
Abstract_Graph map_graph = Abstract_Graph( CLUSTER_XNUM * CLUSTER_YNUM );

///////////////
// Path_Hierarchy class implementation
////////////////////////////////////////
//
Path_Hierarchy::Path_Hierarchy(const int numClusters) {
    _numTrans = 0;
    _numClusters = numClusters;
}

void Path_Hierarchy::addTransition(std::pair<Tile,Tile> tilePair) {
    _transitionS.push_back(tilePair);
    _numTrans++;
}

void Path_Hierarchy::buildClusterS() {

    for(int j = 0; j < CLUSTER_YNUM; j++) {
        for(int i = 0; i < CLUSTER_XNUM; i++)
            _clusterS.push_back(Cluster {i,j, NULL});
    }
}

// Accessors
std::vector<std::pair<Tile, Tile>> Path_Hierarchy::get_transitionS() {return _transitionS;}
std::pair<Tile, Tile> Path_Hierarchy::get_transition(const int transNum) { return _transitionS[transNum] };
std::vector<Cluster> Path_Hierarchy::get_clusterS() {return _clusterS;}
Cluster Path_Hierarchy::get_Cluster(const int x, const int y) {return _clusterS[y*CLUSTER_YNUM + x];}
int Path_Hierarchy::getNumTrans() { return _numTrans; }
int Path_Hierarchy::getNumClusters() { return _numClusters; };

//////////////////////////////////////////

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
            findTransitions(map_hierarchy.get_Cluster(i,j), map_hierarchy.get_Cluster(i,j+1));
        }
    }
    
    for(int j = 0; j < CLUSTER_XNUM; j++) {     // checking all laterally adjacent clusters
        for(int i =0; i < CLUSTER_XNUM - 1; i++) {
            findTransitions(map_hierarchy.get_Cluster(i,j), map_hierarchy.get_Cluster(i+1,j));
        }
    }
}

// Function findTransitions
// takes two adjacent clusters and finds transition points between them
void findTransitions(Cluster c1, Cluster c2) {
    int c1TileX = c1.x*CLUSTER_TLENGTH;
    int c2TileY = c2.y*CLUSTER_TLENGTH;

    std::pair<Tile, Tile> adjTiles;
    Tile l1, l2;
    
    bool adjOrientation(c1.x == c2.x);    // True if c1 on top of c2, False if c1 left of c2 

    bool validEnt(false), currEnt(false);
    int entStart(0), entEnd(0);
    

    for(int i= 0; i <  CLUSTER_TLENGTH; i++) {
        adjTiles = getAdjTiles(c1TileX, c2TileY, i, adjOrientation);
        l1 = adjTiles.first;
        l2 = adjTiles.second;
        
        currEnt = (l1.traversable && l2.traversable);
        if(currEnt && !validEnt) {  // starting a new entrance
            entStart = i;
            validEnt = true;
        }
        else if((!currEnt && validEnt) || (currEnt && validEnt && i == CLUSTER_TLENGTH-1)) {  // end of entrance
            entEnd = i-1;
            if(entStart == entEnd)  // if single-tile entrance, define one transition point
                map_hierarchy.addTransition(getAdjTiles(c1TileX, c2TileY, entStart, adjOrientation));
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

// returns pair of adjacent tiles from cluster pos and tile displacement
std::pair<Tile, Tile> getAdjTiles(int c1TileX, int c2TileY, int k, bool adjOrientation) {
    if(adjOrientation)
        return std::make_pair (getTileFromTPos(k+c1TileX, c2TileY-1), getTileFromTPos(k+c1TileX, c2TileY));
    else
        return std::make_pair (getTileFromTPos(c1TileX-1, k+c2TileY), getTileFromTPos(c1TileX, k+c2TileY));
}




// DUMMY FUNCTION, NEEDS TO GET INFO FROM MAP
Tile getTileFromTPos(int x, int y) {
    // CODE HERE GETS TILE INFO FROM MAP
    return Tile {x, y, NULL, true};
}

// Inserts Cluster Transition Tiles into Abstract Graph and sets distances between them as edges
void buildGraph() {
    int cNum1, cNum2;
    Vertex v1, v2;
    std::vector<Vertex*> dummyV;
    std::vector<Edge*> dummyE;
    std::pair<Tile,Tile> transition;
    int numTrans = map_hierarchy.getNumTrans();
    int numClusters = map_hierarchy.getNumClusters();
    for (int i = 0; i < numTrans; i++) {
        transition = map_hierarchy.get_transition(i);
        cNum1 = getClusterNum(*transition.first.parent);
        cNum2 = getClusterNum(*transition.second.parent);
        v1 = Vertex(map_graph.getVCNum(c1), transition.first&, dummyV, dummyE);
        v2 = Vertex(map_graph.getVCNum(c2), transition.second&, dummyV, dummyE);
        map_graph.addVertex(v1, c1);
        map_graph.addVertex(v2, c2);
        map_graph.addEdge(v1.key, v2.key, cNum1, cNum2, 1, INTER);
    }
    int vNum;
    double distance;
    for (int cNum = 0; cNum < numClusters; cNum++) {
        numVC = map_graph.getVCNum(cNum);
        for (int i = 0; i < numVC; i++) {
            for (int j = 0; j < numVC; j++) {
                if (i == j)
                    continue;
                v1 = map_graph.getVertexCopy(i, cNum);
                v2 = map_graph.getVertexCopy(j, cNum);
                distance = map_graph.searchForDistance(v1, v2);
                if (distance != HUGE_VAL)
                    map_graph.addEdge(v1.key, v2.key, cNum, cNum, distance, INTRA);
            }
        }
    }
}


// Not considering more than one level at this point... difficult implementation + increased memory requirements for minimal performance upgrades considering our application
/*
void addLevelToGraph(int l) {

}
*/


void preprocessing(int maxLevel) {
    abstractMap();
    buildGraph();

}