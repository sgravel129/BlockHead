#pragma once

#include "constants.hpp"
#include "map.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <queue>



typedef struct Cluster {
    Point clusterPos;
    Point tilePos;
    Cluster* parent;
} Cluster;

class Path_Hierarchy;

class PathTile {
public:
    PathTile(); // dummy initialization constructor
    PathTile(const MapTile&, Path_Hierarchy&, const int, const int);    // constructor for A * algorithm
    PathTile(const MapTile&, Path_Hierarchy&);  // constructor for encapsulated transition points
    PathTile(const PathTile& t2);   // copy constructor

    Cluster* findParent();
    void updatePriority(const int xDest, const int yDest);
    void nextLevel(const int i);
    const int estimate(const int xDest, const int yDest) const;

    // Accessors
    Point get_mapRelPos() const;
    Point get_clusterRelPos() const;
    bool get_traversable() const;
    int get_level() const;
    int get_priority() const;
    Cluster getParentCCopy() const;   // returns copy of parent cluster
    Cluster* getParentC();  // returns address of parent cluster
    
private:    
    Point _mapRelPos;
    Point _clusterRelPos;
    bool _traversable;   // True if no obstacle on tile, False o/w
    int _level;
    int _priority;
    
    Cluster* _parentC;
    Path_Hierarchy* _parentH;
};

bool operator<(const PathTile& LHS, const PathTile& RHS);
std::vector<int> pathFind(const Point, const Point, const int);     // A STAR ALGORITHM
double pathToDistance(const std::vector<int>&);

class Path_Hierarchy {
public:
    Path_Hierarchy(const int numClusters);
    
    void addTransition(const std::pair<PathTile*, PathTile*>& tilePair);
    void buildClusterS();
    //std::pair<PathTile, PathTile> getAdjTiles(const Cluster&, const Cluster&, const int, const bool);

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

Point getClusterPoint(const int);
int getClusterNum(const Point&);
void findTransitions(const Cluster& c1, const Cluster& c2);
std::pair<PathTile*, PathTile*> getAdjTiles(const Cluster&, const Cluster&, const int, const bool);
PathTile* getPathTileFromPoint(const Point& p);
MapTile* getMapTileFromPoint(const Point& p);

void abstractMap();
void buildGraph();
void preprocessing();

