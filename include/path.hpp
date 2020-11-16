#pragma once

#include "constants.hpp"
#include "map.hpp"

#include <vector>
#include <cmath>





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
    void updatePriority(const int xDest, const int yDest);
    void nextLevel(const int i);
    const int estimate(const int xDest, const int yDest) const;

    // Accessors
    int get_xPos() const;
    int get_yPos() const;
    bool get_traversable() const;
    int get_level() const;
    int get_priority() const;
    Cluster getParentCCopy() const;   // returns copy of parent cluster
    Cluster* getParentC();  // returns address of parent cluster
    
private:    
    int _xPos;
    int _yPos;
    bool _traversable;   // True if no obstacle on tile, False o/w
    int _level;
    int _priority;
    
    Cluster* _parentC;
    Path_Hierarchy* _parentH;
};

bool operator<(const PathTile& LHS, const PathTile& RHS);


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
