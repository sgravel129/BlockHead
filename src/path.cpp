/*
#include "../include/path.hpp"
#include "../include/constants.hpp"
#include "../include/graph.hpp"
*/
#include "path.hpp"
#include "constants.hpp"
#include "graph.hpp"
#include "log.hpp"

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

const int dir = 8;  // number of possible directions to go in
const int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };              // dx and dy representing 8 cardinal directions, starting East then proceeding clockwise
const int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };
static int intMap[CLUSTER_XNUM][CLUSTER_YNUM][CLUSTER_TLENGTH][CLUSTER_TLENGTH];


// TODO
// Implement Destructor
// Test
// Integrate with rest of build
// Define and add Zombie spawns to abstract graph


// dummy global variable: might want to declare this in separate file, which is run when we load a map
Path_Hierarchy map_hierarchy = Path_Hierarchy(CLUSTER_XNUM * CLUSTER_YNUM);
Abstract_Graph map_graph = Abstract_Graph( CLUSTER_XNUM * CLUSTER_YNUM );
Map game_map = Map();

// Need to add accessor for _tiles in Map class
// accesses map info
PathTile* getPathTileFromPoint(const Point& p) {
    //return PathTile(game_map.get_tile(p), map_hierarchy);
    MapTile mt;
    return new PathTile(mt, map_hierarchy);    // dummy return for now
}
MapTile* getMapTileFromPoint(const Point& p) {
    return new MapTile;  // dummy return for now
}


/////////////////////
// PathTile class implementation
///////////////////////////////////////
// Constructors
PathTile::PathTile() : _mapRelPos{}, _clusterRelPos{}, _traversable{}, _level{}, _priority{}, _parentC{}, _parentH{} {};
PathTile::PathTile(const MapTile& refTile, Path_Hierarchy& parentH) : PathTile(refTile, parentH, 0, 0) {}
PathTile::PathTile(const MapTile& refTile, Path_Hierarchy& parentH, const int level, const int priority) {
    _mapRelPos = { refTile.getPos().x, refTile.getPos().y };
    _traversable = !refTile.getCollision();
    _level = level;
    _priority = priority;    
    _parentH = &parentH;
    _parentC = findParent();
    _clusterRelPos = {};
}
PathTile::PathTile(const PathTile& t2) {        // Copy Constructor
    _mapRelPos = t2._mapRelPos; _clusterRelPos = t2._clusterRelPos; _traversable = t2._traversable;
    _level = t2._level; _priority = t2._priority; _parentC = t2._parentC; _parentH = t2._parentH;
}
// Destructor
PathTile::~PathTile() { // set pointers to parent cluster and hierarchy to null to avoid leaks
    _parentC = NULL;
    _parentH = NULL;
}
///////////////////
// Member Functions
// Called by constructor, finds parent cluster from tile point
Cluster* PathTile::findParent() {
    Point p = { _mapRelPos.x / CLUSTER_TLENGTH , _mapRelPos.y / CLUSTER_TLENGTH };
    return _parentH->getClusterAddress(p);
}
// Update A * priority
void PathTile::updatePriority(const int xDest, const int yDest) {
    _priority = _level + estimate(xDest, yDest) * 10;
}
// Update A * level
void PathTile::nextLevel(const int i) { // i : direction
    _level += (dir == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
}
// Find estimate distance to supplied destination
const int PathTile::estimate(const int xDest, const int yDest) const {
    int xd, yd, d;
    xd = xDest - _clusterRelPos.x;
    yd = yDest - _clusterRelPos.y;
    d = static_cast<int>(sqrt(xd * xd + yd * yd));
    return(d);
}


// Accessors
Point PathTile::get_mapRelPos() const { return _mapRelPos; }
Point PathTile::get_clusterRelPos() const { return _clusterRelPos; }
bool PathTile::get_traversable() const { return _traversable; }
int PathTile::get_level() const { return _level;  }
int PathTile::get_priority() const { return _priority; }
Cluster PathTile::getParentCCopy() const { return *_parentC; }

// Memory Accessors
Cluster* PathTile::getParentC() { return _parentC; }
/////////////////////////////////////////////////////////////////////////////
// PathTile operator overload
bool operator<(const PathTile& LHS, const PathTile& RHS) {
    return LHS.get_priority() > RHS.get_priority();
}
// A STAR ALGORITHM
std::vector<int> pathFind(const Point startP, const Point finishP, const int cNum) {
    Point clusterP = getClusterPoint(cNum);
    const int xStart = startP.x, yStart = startP.y;         // x and y values in cluster relative terms
    const int xFinish = finishP.x, yFinish = finishP.y;
    int closed_nodes_map[CLUSTER_TLENGTH][CLUSTER_TLENGTH];
    int open_nodes_map[CLUSTER_TLENGTH][CLUSTER_TLENGTH];
    int dir_map[CLUSTER_TLENGTH][CLUSTER_TLENGTH];
    std::priority_queue<PathTile> pq[2];
    int pqi = 0;
    PathTile* n0;
    PathTile* m0;
    MapTile* tempMT;
    int i, j, x, y, xdx, ydy;
    std::vector<int> path;
    char c;
    // create the start node and push into list of open nodes
    n0 = new PathTile(*getMapTileFromPoint({ xStart+clusterP.x, yStart+clusterP.y }), map_hierarchy, 0, 0);
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    open_nodes_map[xStart][yStart] = n0->get_priority(); // mark it on the open nodes map

    // A* search
    while (!pq[pqi].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        n0 = new PathTile(pq[pqi].top());
        x = n0->get_clusterRelPos().x; y = n0->get_clusterRelPos().y;

        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map[x][y] = 0;
        // mark it on the closed nodes map
        closed_nodes_map[x][y] = 1;

        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if (x == xFinish && y == yFinish)
        {
            // generate the path from finish to start
            // by following the directions
            //std::string path = "";
            while (!(x == xStart && y == yStart))
            {
                j = dir_map[x][y];
                path.insert(path.begin(), ((j + dir / 2) % dir));
                //c = '0' + (j + dir / 2) % dir;
                //path = c + path;
                x += dx[j];
                y += dy[j];
            }

            // garbage collection
            delete n0;
            // empty the leftover nodes
            while (!pq[pqi].empty()) pq[pqi].pop();
            return path;
        }

        // generate moves (child nodes) in all possible directions
        for (i = 0; i < dir; i++)
        {
            xdx = x + dx[i]; ydy = y + dy[i];

            if (!(xdx<0 || xdx> CLUSTER_TLENGTH - 1 || ydy<0 || ydy > CLUSTER_TLENGTH - 1 || intMap[clusterP.x][clusterP.y][xdx][ydy] == 1
                || closed_nodes_map[xdx][ydy] == 1))
            {
                // generate a child node
                tempMT = getMapTileFromPoint({ clusterP.x+xdx, clusterP.y+ydy });     // get MapTile for new possible tile to move to
                if (tempMT->getCollision()) {                   // if an obstacle, update map data and go to next loop iteration
                    intMap[clusterP.x][clusterP.y][xdx][ydy] = 1;
                    continue;
                }
                m0 = new PathTile(*tempMT, map_hierarchy, n0->get_level(), n0->get_priority());
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if (open_nodes_map[xdx][ydy] == 0)
                {
                    open_nodes_map[xdx][ydy] = m0->get_priority();
                    pq[pqi].push(*m0);
                    // mark its parent node direction
                    dir_map[xdx][ydy] = (i + dir / 2) % dir;
                }
                else if (open_nodes_map[xdx][ydy] > m0->get_priority())
                {
                    // update the priority info
                    open_nodes_map[xdx][ydy] = m0->get_priority();
                    // update the parent direction info
                    dir_map[xdx][ydy] = (i + dir / 2) % dir;

                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while (!(pq[pqi].top().get_clusterRelPos().x == xdx &&
                        pq[pqi].top().get_clusterRelPos().y == ydy))
                    {
                        pq[1 - pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop(); // remove the wanted node

                    // empty the larger size pq to the smaller one
                    if (pq[pqi].size() > pq[1 - pqi].size()) pqi = 1 - pqi;
                    while (!pq[pqi].empty())
                    {
                        pq[1 - pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi = 1 - pqi;
                    pq[pqi].push(*m0); // add the better node instead
                }
                else delete m0; // garbage collection
            }
        }
        delete n0; // garbage collection
    }
    return path; // no route found
}


double pathToDistance(const std::vector<int>& path) {
    if (path.size() == 0)
        return HUGE_VAL;
    double d = 0;
    for (int i = 0; i < path.size(); i++) {
        if (path[i] % 2 == 1)
            d += 1.414;
        else
            d++;
    }
    return d;
}




///////////////
// Path_Hierarchy class implementation
////////////////////////////////////////
//
Path_Hierarchy::Path_Hierarchy(const int numClusters) {
    _numTrans = 0;
    _numClusters = numClusters;
}

void Path_Hierarchy::addTransition(const std::pair<PathTile*, PathTile*>& tilePair) {
    _transitionS.push_back(std::pair<PathTile, PathTile> {*tilePair.first, *tilePair.second});
    _numTrans++;
}

void Path_Hierarchy::buildClusterS() {
    for(int j = 0; j < CLUSTER_YNUM; j++) {
        for(int i = 0; i < CLUSTER_XNUM; i++)
            _clusterS.push_back(Cluster{ {i,j}, {i*CLUSTER_TLENGTH, j*CLUSTER_TLENGTH}, NULL });
    }
}
// Destructor
Path_Hierarchy::~Path_Hierarchy() {
    deleteStartAndGoal();
    PathTile* temp1, * temp2;
    Cluster* tempC;
    for (int i = 0; i < _numTrans; i++) {
        getTransitionTileAddresses(i, temp1, temp2);
        delete temp1;
        delete temp2;
    }
    _transitionS.~vector();
    for (int i = 0; i < _numClusters; i++) {
        tempC = getClusterAddress(getClusterPoint(i));
        delete tempC;
    }
    _clusterS.~vector();
}

// returns pair of adjacent tiles from cluster pos and tile displacement
std::pair<PathTile*, PathTile*> getAdjTiles(const Cluster& c1, const Cluster& c2, const int k, const bool adjOrientation) {
    Point p1, p2;
    if (adjOrientation) {   // Cluster 1 on left of Cluster 2
        p1.x = k + c1.tilePos.x; p1.y = c2.tilePos.y - 1;
        p2.x = k + c1.tilePos.x; p2.y = c2.tilePos.y;
        return std::make_pair(getPathTileFromPoint(p1), getPathTileFromPoint(p2));
    }
    else {                  // Cluster 1 on top of Cluster 2
        p1.x = c1.tilePos.x; p1.y = k + c2.tilePos.y;
        p2.x = c1.tilePos.x + 1; p2.y = k + c2.tilePos.y;
        return std::make_pair(getPathTileFromPoint(p1), getPathTileFromPoint(p2));
    }
}


// Accessors
std::vector<std::pair<PathTile, PathTile>> Path_Hierarchy::get_transitionS() { return _transitionS; }
std::pair<PathTile, PathTile> Path_Hierarchy::get_transition(const int transNum) { return _transitionS[transNum]; }
std::vector<Cluster> Path_Hierarchy::get_clusterS() {return _clusterS;}
Cluster Path_Hierarchy::get_cluster(const int x, const int y) {return _clusterS[y*CLUSTER_YNUM + static_cast<__int64>(x)];}
int Path_Hierarchy::get_numTrans() { return _numTrans; }
int Path_Hierarchy::get_numClusters() { return _numClusters; }

// Private Memory Accessors
Cluster* Path_Hierarchy::getClusterAddress(const Point& p) {
    return &(_clusterS[p.y * CLUSTER_YNUM + static_cast<__int64>(p.x)]);
}

bool Path_Hierarchy::getTransitionTileAddresses(const int transNum, PathTile*& t1, PathTile*& t2) {
    if (transNum >= _numTrans)
        return false;
    t1 = &(_transitionS[transNum].first);
    t2 = &(_transitionS[transNum].second);
    return true;
}

// Deletion Mutators
void Path_Hierarchy::deleteStartAndGoal() {
    _startT.~PathTile();
    _goalT.~PathTile();
}

//////////////////////////////////////////////////////////////////////////
// get cluster Point from Cluster Num
Point getClusterPoint(const int clusterNum) {
    int x = clusterNum % CLUSTER_YNUM;
    int y = (clusterNum - x) / CLUSTER_YNUM;
    return { x, y };
}
// gets cluster index in _clusterS vector
int getClusterNum(const Point& p) {
    return(p.y*CLUSTER_YNUM + p.x);
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

    std::pair<PathTile*, PathTile*> adjTiles;
    PathTile* l1, * l2;
    
    bool adjOrientation(c1.clusterPos.x == c2.clusterPos.x);    // True if c1 on top of c2, False if c1 left of c2 

    bool validEnt(false), currEnt(false);
    int entStart(0), entEnd(0);
    

    for(int i = 0; i <  CLUSTER_TLENGTH; i++) {
        adjTiles = getAdjTiles(c1, c2, i, adjOrientation);
        l1 = adjTiles.first;
        l2 = adjTiles.second;
        
        currEnt = (l1->get_traversable() && l2->get_traversable());
        if(currEnt && !validEnt) {  // starting a new entrance
            entStart = i;
            validEnt = true;
        }
        else if((!currEnt && validEnt) || (currEnt && validEnt && i == CLUSTER_TLENGTH-1)) {  // end of entrance
            entEnd = i-1;
            if (entStart == entEnd) {  // if single-tile entrance, define one transition point
                
                map_hierarchy.addTransition(getAdjTiles(c1, c2, entStart, adjOrientation));
            }
            else if(entEnd - entStart < MIN_ENTRANCE_LENGTH) {  // if small entrance, define one transition point in middle
                
                map_hierarchy.addTransition(getAdjTiles(c1, c2, entStart + static_cast<int>((entEnd-entStart)/2), adjOrientation));
            }
            else {      // otherwise define multiple transition points
                for(int k = entStart; k < entEnd+1; k += TRANSITION_INTERVAL)
                    
                    map_hierarchy.addTransition(getAdjTiles(c1, c2, k, adjOrientation));
            }
        }
    }   
}


// Inserts Cluster Transition Tiles into Abstract Graph and sets distances between them as edges
// NOTE Might be a good idea to define zombie spawns, and add them to the graph
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
        cNum1 = getClusterNum(t1->getParentCCopy().clusterPos);
        cNum2 = getClusterNum(t2->getParentCCopy().clusterPos);
        v1 = { map_graph.getVCNum(cNum1), t1, dummyV, dummyE };
        v2 = { map_graph.getVCNum(cNum2), t2, dummyV, dummyE };
        map_graph.addVertex(v1, cNum1);
        map_graph.addVertex(v2, cNum2);
        if(v1.t->getParentCCopy().clusterPos.x == v2.t->getParentCCopy().clusterPos.x)  // if v1 and v2 horizontally adjacent
            map_graph.addEdge(v1.key, v2.key, cNum1, cNum2, 1, INTER, { 0 });     // {0} is the path of going right once
        else                                                                            // if v1 and v2 vertically adjacent
            map_graph.addEdge(v1.key, v2.key, cNum1, cNum2, 1, INTER, { 2 });     // {2} is the path of going down once
    }
    int numVC;
    double distance;
    std::vector<int> path;
    for (int cNum = 0; cNum < numClusters; cNum++) {
        numVC = map_graph.getVCNum(cNum);
        for (int i = 0; i < numVC; i++) {
            for (int j = 0; j < numVC; j++) {
                if (i == j)
                    continue;
                if (!(map_graph.getVertexCopy(i, cNum, v1) && map_graph.getVertexCopy(j, cNum, v2)))
                    break;
                path = pathFind(v1.t->get_clusterRelPos(), v2.t->get_clusterRelPos(), cNum);
                distance = pathToDistance(path);
                if (distance != HUGE_VAL)
                    map_graph.addEdge(v1.key, v2.key, cNum, cNum, distance, INTRA, path);
            }
        }
    }
}


void preprocessing() {
    abstractMap();
    buildGraph();
}