#pragma once

#include "constants.hpp"
#include "map.hpp"

#include <vector>
#include <cmath>


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

typedef struct Cluster {
    int x;
    int y;
    Cluster* parent;
} Cluster;

class Path_Hierarchy;

class PathTile {
public:
    PathTile();
    PathTile(const MapTile&, Path_Hierarchy&);

    Cluster* findParent();
    // Accessors
    int get_xPos();
    int get_yPos();
    bool get_traversable();
    Cluster getParentCCopy();   // returns copy of parent cluster
    Cluster* getParentC();  // returns address of parent cluster
    
private:    
    int _xPos;
    int _yPos;
    bool _traversable;   // True if no obstacle on tile, False o/w
    Cluster* _parentC;
    Path_Hierarchy* _parentH;

    
};


class Path_Hierarchy {
public:
    Path_Hierarchy(const int numClusters);
    
    void addTransition(const std::pair<PathTile, PathTile>& tilePair);
    void buildClusterS();
    std::pair<PathTile, PathTile> getAdjTiles(const int, const int, const int, const bool);

    // Accessors
    std::vector<std::pair<PathTile, PathTile>> get_transitionS();
    std::pair<PathTile, PathTile> get_transition(const int);
    std::vector<Cluster> get_clusterS();
    Cluster get_cluster(const int, const int);   // returns copy of cluster located at supplied x,y tile pos

    int get_numTrans();
    int get_numClusters();

    Cluster* getClusterAddress(const int, const int);      /// returns pointer to encapsulated cluster
    bool getTransitionTileAddresses(const int, PathTile*&, PathTile*&);

private:
    std::vector<std::pair<PathTile, PathTile>> _transitionS;
    std::vector<Cluster> _clusterS;
    int _numTrans;
    int _numClusters;


};


int getClusterNum(const Cluster& c);
void findTransitions(const Cluster& c1, const Cluster& c2);
std::pair<PathTile, PathTile> getAdjTiles(const int c1TileX, const int c2TileY, const int k, const bool adjOrientation);
PathTile getPathTileFromPoint(const Point& p);


void abstractMap();
void buildGraph();
void preprocessing(int maxLevel);
