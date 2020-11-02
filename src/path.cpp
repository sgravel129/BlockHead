#include "path.hpp"
#include "constants.hpp"
#include <vector>

// dummy global variable: might want to declare this in separate file, which is run when we load a map
Path_Hierarchy map_hierarchy;


///////////////
// Path_Hierarchy class implementation
////////////////////////////////////////
//
Path_Hierarchy::Path_Hierarchy() {
    std::vector<std::pair<Tile, Tile>> _transitionS;
    std::vector<std::vector<Cluster>> _clusterS;
}

void Path_Hierarchy::addTransition(std::pair<Tile,Tile> tilePair) {
    _transitionS.push_back(tilePair);
}

void Path_Hierarchy::buildClusterS(const int cluster_xNum, const int cluster_yNum) {
    std::vector<Cluster> tempRow;

    for(int i = 0; i < cluster_xNum; i++) {
        for(int j = 0; j < cluster_yNum; j++)
            tempRow.push_back(Cluster {i,j, NULL});
        _clusterS.push_back(tempRow);
    }
}

// Accessors
std::vector<std::pair<Tile, Tile>> Path_Hierarchy::get_transitionS() {return _transitionS;};
std::vector<std::vector<Cluster>> Path_Hierarchy::get_clusterS() {return _clusterS;};
Cluster Path_Hierarchy::get_Cluster(const int x, const int y) {return _clusterS[x][y];};


//////////////////////////////////////////


// Function abstractMap
// divides map into clusters and defines transition points between them
void abstractMap() {
    
    const int cluster_xNum = X_MAX/CLUSTER_SLENGTH;
    const int cluster_yNum = Y_MAX/CLUSTER_SLENGTH;
    map_hierarchy.buildClusterS(cluster_xNum, cluster_yNum);


    // Adding entrances between clusters for all adjacent clusters
    for(int i = 0; i < cluster_xNum; i++) {     // checking all vertically adjacent clusters
        for(int j =0; j <cluster_yNum-1; j++) {
            findTransitions(map_hierarchy.get_Cluster(i,j), map_hierarchy.get_Cluster(i,j+1));
        }
    }
    
    for(int j = 0; j < cluster_yNum; j++) {     // checking all laterally adjacent clusters
        for(int i =0; i <cluster_xNum-1; i++) {
            findTransitions(map_hierarchy.get_Cluster(i,j), map_hierarchy.get_Cluster(i+1,j));
        }
    }


}

// Function findTransitions
// takes two adjacent clusters and finds transition points between them
void findTransitions(Cluster c1, Cluster c2) {
    int c1TileX = c1.x*CLUSTER_SLENGTH;
    int c2TileY = c2.y*CLUSTER_SLENGTH;

    std::pair<Tile, Tile> adjTiles;
    Tile l1, l2;
    
    bool adjOrientation(c1.x == c2.x);    // True if c1 on top of c2, False if c1 left of c2 

    bool validEnt(false), currEnt(false);
    int entStart(0), entEnd(0);
    

    for(int i= 0; i <  CLUSTER_SLENGTH; i++) {
        adjTiles = getAdjTiles(c1TileX, c2TileY, i, adjOrientation);
        l1 = adjTiles.first;
        l2 = adjTiles.second;
        
        currEnt = (l1.traversable && l2.traversable);
        if(currEnt && !validEnt) {  // starting a new entrance
            entStart = i;
            validEnt = true;
        }
        else if((!currEnt && validEnt) || (currEnt && validEnt && i == CLUSTER_SLENGTH-1)) {  // end of entrance
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

// INCOMPLETE FUNCTION
void buildGraph() {
    Cluster c1, c2;
    for(auto transition : map_hierarchy.get_transitionS()) {
        c1 = *transition.first.parent;
        c2 = *transition.second.parent;
        
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