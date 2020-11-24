#pragma once


#include "map.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <queue>

class Path_Hierarchy;


typedef int* intArrayPtr;

typedef struct Cluster {
    Point clusterPos{};
    Point tilePos{};
} Cluster;



class PathTile {
public:
    PathTile(); // dummy initialization constructor
    PathTile(const Point&, Path_Hierarchy*, const int, const int);    // constructor for A * algorithm
    PathTile(const Point&, const bool, Path_Hierarchy*, const int, const int);    // constructor for A * algorithm
    PathTile(const Point&, Path_Hierarchy*);  // constructor for encapsulated transition points
    PathTile(const Point&, const bool, Path_Hierarchy*);  // constructor for encapsulated transition points

    PathTile(const PathTile& t2);   // copy constructor

    
    void updatePriority(const int xDest, const int yDest);
    void nextLevel(const int i);
    const int estimate(const int xDest, const int yDest) const;

    // Accessors
    Point get_mapRelPos() const;
    Point get_clusterRelPos() const;
    bool get_traversable() const;
    int get_level() const;
    int get_priority() const;
    Cluster getParentCluster() const;   // returns copy of parent cluster
    
    // Destructors
    ~PathTile();

private:    
    Point _mapRelPos;
    Point _clusterRelPos;
    bool _traversable;   // True if no obstacle on tile, False o/w
    int _level;
    int _priority;
    
    Cluster _parentC;
    Path_Hierarchy* _parentH;
};

bool operator<(const PathTile& LHS, const PathTile& RHS);
std::vector<int> pathFind(const Point, const Point, const int);     // A STAR ALGORITHM, takes cluster relative points
                                                                    // of start and goal, and cluster number
double pathToDistance(const std::vector<int>&);

class Path_Hierarchy {
public:
    Path_Hierarchy(const int numClusters);
    
    void addTransition(const std::pair<PathTile*, PathTile*>& tilePair);
    void addStart(PathTile* const);
    void addGoal(PathTile* const);
    void buildClusterS();
    

    // Accessors
    //std::vector<std::pair<PathTile*, PathTile*>> get_transitionS();
    std::pair<PathTile, PathTile> get_transition(const int);
    std::vector<Cluster> get_clusterS();
    Cluster get_cluster(const Point);   // returns copy of cluster located at supplied x,y tile pos
    Cluster get_cluster(const int); // returns copy of cluster for supplied cNum

    int get_numTrans();
    int get_numClusters();

    // Memory Accessors
    bool getTransitionTileAddresses(const int, PathTile*&, PathTile*&);
    bool getStartAddress(PathTile*& t);
    bool getGoalAddress(PathTile*& t);

    // Destructors
    void deleteStartAndGoal();
    ~Path_Hierarchy();

private:
    std::vector<std::pair<PathTile*, PathTile*>> _transitionS;
    std::vector<Cluster> _clusterS;
    int _numTrans;
    int _numClusters;
    PathTile* _startT;
    PathTile* _goalT;

};

Point getClusterPFromNum(const int);
int getClusterNum(const Point&);
void findTransitions(const Cluster& c1, const Cluster& c2);
std::pair<PathTile*, PathTile*> getAdjTiles(const Cluster&, const Cluster&, const int, const bool);
PathTile* getPathTileFromPoint(const Point& p);
//MapTile* getMapTileFromPoint(const Point& p);


std::vector<int> searchForPath(const Point& startP, const Point& goalP);

void abstractMap();
void buildGraph();
void buildGraphPaths();
void preprocessing();

typedef int vertex_t;
typedef double weight_t;

struct neighbor {
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) { }
};

typedef std::vector<std::vector<neighbor> > adjacency_list_t;


Cluster findParentCluster(const Point&);