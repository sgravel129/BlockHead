
#include <vector>
#include "constants.hpp"
#include <cmath>

#define HUGE_VAL

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


class Path_Hierarchy {
public:
    Path_Hierarchy(const int numClusters);
    
    void addTransition(std::pair<Tile,Tile> tilePair);
    void buildClusterS();

    // Accessors
    std::vector<std::pair<Tile, Tile>> get_transitionS();
    std::pair<Tile, Tile> get_transition(const int);
    std::vector<Cluster> get_clusterS();
    Cluster get_Cluster(const int, const int);
    int getNumTrans();
    int getNumClusters ();

private:
    std::vector<std::pair<Tile, Tile>> _transitionS;
    std::vector<Cluster> _clusterS;
    int _numTrans;
    int _numClusters;

};


typedef struct Tile {

    int x;
    int y;
    Cluster* parent;
    bool traversable;   // True if no obstacle on tile, False o/w
} Tile;

typedef struct Cluster {
    int x;
    int y;
    Cluster* parent;
} Cluster;

void abstractMap();
void buildGraph();
void preprocessing(int maxLevel);

int getClusterNum(const Cluster& c);