
#include <vector>
#include "constants.hpp"

#define X_MAX SCREEN_WIDTH
#define Y_MAX SCREEN_HEIGHT
// will change these depending on amount of pixels sprite occupies
//#define CLUSTER_WIDTH 120
//#define CLUSTER_HEIGHT 120
#define CLUSTER_SLENGTH 120     // side length of cluster (square)
#define X_STEP 30   // x size of tile
#define Y_STEP 30   // y size of tile
#define MIN_ENTRANCE_LENGTH 3   // minimum tile lenght for entrance
#define TRANSITION_INTERVAL 3   // number of tiles between transitions in single entrance


class Path_Hierarchy {
public:
    Path_Hierarchy();
    
    void addTransition(std::pair<Tile,Tile> tilePair);
    void buildClusterS(const int, const int);

    // Accessors
    std::vector<std::pair<Tile, Tile>> get_transitionS();
    std::vector<std::vector<Cluster>> get_clusterS();
    Cluster get_Cluster(const int, const int);

private:
    std::vector<std::pair<Tile, Tile>> _transitionS;
    std::vector<std::vector<Cluster>> _clusterS;

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

