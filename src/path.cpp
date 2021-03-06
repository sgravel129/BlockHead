#include "path.hpp"
#include "graph.hpp"
#include "dijkstra.hpp"
#include "pathRes.hpp"
#include "log.hpp"

#include <vector>
#include <cmath>

#define TLENGTH 75

#define X_MAX 3000
#define Y_MAX 1800

#define CLUSTER_TLENGTH 8
#define CLUSTER_XNUM 5
#define CLUSTER_YNUM 3


#define MIN_ENTRANCE_LENGTH 3   // minimum tile lenght for entrance
#define TRANSITION_INTERVAL 3   // number of tiles between transitions in single entrance

using namespace std;

const int dir = 8;  // number of possible directions to go in
const int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };              // dx and dy representing 8 cardinal directions, starting East then proceeding clockwise
const int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };
static int intMap[CLUSTER_XNUM][CLUSTER_YNUM][CLUSTER_TLENGTH][CLUSTER_TLENGTH];

const weight_t max_weight = std::numeric_limits<double>::infinity();


// TODO
// Integrate with rest of build


// dummy global variable: might want to declare this in separate file, which is run when we load a map

GlobalPathVars* GP = new GlobalPathVars;
/*
Path_Hierarchy* map_hierarchy = new Path_Hierarchy(CLUSTER_XNUM * CLUSTER_YNUM);
Abstract_Graph* map_graph = new Abstract_Graph( CLUSTER_XNUM * CLUSTER_YNUM );
Map* game_map = new Map();
*/
// Need to add accessor for _tiles in Map class
// accesses map info
bool getTileCol(const Point& p) {

    return GP->_collisionM[p.x][p.y];
}


PathTile* getPathTileFromPoint(const Point& p) {
    //return PathTile(game_map.get_tile(p), map_hierarchy);
    //MapTile mt;
    return new PathTile(p, getTileCol(p), GP->map_hierarchy);    // dummy return for now
}
/*
MapTile* getMapTileFromPoint(const Point& p) {

    return new MapTile;  // dummy return for now
}
*/


/////////////////////
// PathTile class implementation
///////////////////////////////////////
// Constructors
PathTile::PathTile() : _mapRelPos{}, _clusterRelPos{}, _traversable{}, _level{}, _priority{}, _parentC{}, _parentH(nullptr) {};
PathTile::PathTile(const Point& p, Path_Hierarchy* parentH) : PathTile(p, getTileCol(p), parentH) {}
PathTile::PathTile(const Point& p, Path_Hierarchy* parentH, const int level, const int priority) : PathTile(p, getTileCol(p), parentH, level, priority) {}
PathTile::PathTile(const Point& p, const bool trav, Path_Hierarchy* parentH) : PathTile(p, trav, parentH, 0, 0) {}
PathTile::PathTile(const Point& p, const bool trav, Path_Hierarchy* parentH, const int level, const int priority) {
    _mapRelPos = p;
    _clusterRelPos = { _mapRelPos.x % static_cast<int32_t>(CLUSTER_TLENGTH), _mapRelPos.y % static_cast<int32_t>(CLUSTER_TLENGTH) };
    _traversable = trav;
    _level = level;
    _priority = priority;
    _parentH = parentH;
    _parentC = findParentCluster(_mapRelPos);
    true;
}
PathTile::PathTile(const PathTile& t2) {        // Copy Constructor
    _mapRelPos = t2._mapRelPos; _clusterRelPos = t2._clusterRelPos; _traversable = t2._traversable;
    _level = t2._level; _priority = t2._priority; _parentC = t2._parentC; _parentH = t2._parentH;
}
// Destructor
PathTile::~PathTile() { // set pointer to hierarchy to null to avoid leaks
    _parentH = nullptr;
}
///////////////////
// Member Functions

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
Cluster PathTile::getParentCluster() const { return _parentC; }


/////////////////////////////////////////////////////////////////////////////
// PathTile operator overload
bool operator<(const PathTile& LHS, const PathTile& RHS) {
    return LHS.get_priority() > RHS.get_priority();
}
// A STAR ALGORITHM
std::vector<int> pathFind(const Point startP, const Point finishP, const int cNum) {
    std::vector<std::vector<bool>> lmaoxd = GP->_collisionM;
    Cluster c = GP->map_hierarchy->get_cluster(cNum);
    const int xStart = startP.x, yStart = startP.y;         // x and y values in cluster relative terms
    const int xFinish = finishP.x, yFinish = finishP.y;
    int closed_nodes_map[CLUSTER_TLENGTH][CLUSTER_TLENGTH];
    int open_nodes_map[CLUSTER_TLENGTH][CLUSTER_TLENGTH];
    int dir_map[CLUSTER_TLENGTH][CLUSTER_TLENGTH];
    std::priority_queue<PathTile> pq[2];
    int pqi = 0;
    PathTile* n0;
    PathTile* m0;
    //MapTile* tempMT;
    int i, j, x, y, xdx, ydy;
    std::vector<int> path;
    // reset the node maps
    for (y = 0; y < CLUSTER_TLENGTH; y++)
    {
        for (x = 0; x < CLUSTER_TLENGTH; x++)
        {
            closed_nodes_map[x][y] = 0;
            open_nodes_map[x][y] = 0;
        }
    }



    // create the start node and push into list of open nodes
    Point p = { xStart + c.tilePos.x, yStart + c.tilePos.y };
    //tempMT = getMapTileFromPoint({ xStart + clusterP.x, yStart + clusterP.y });
    n0 = new PathTile(p, GP->map_hierarchy, 0, 0);

    // Check if start or goal tile is traversable;
    if (!n0->get_traversable()) return {};
    p = { xFinish + c.tilePos.x, yFinish + c.tilePos.y };
    PathTile* goalT = new PathTile(p, GP->map_hierarchy, 0, 0);
    if (!goalT->get_traversable()) return {};
    delete goalT;
    goalT = nullptr;

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

            if (!(xdx<0 || xdx> CLUSTER_TLENGTH - 1 || ydy<0 || ydy > CLUSTER_TLENGTH - 1 || intMap[c.clusterPos.x][c.clusterPos.y][xdx][ydy] == 1
                || closed_nodes_map[xdx][ydy] == 1))
            {
                // generate a child node
                //tempMT = getMapTileFromPoint({ clusterP.x+xdx, clusterP.y+ydy });     // get MapTile for new possible tile to move to
                p = { c.tilePos.x + xdx, c.tilePos.y + ydy };
                if (!getTileCol(p)) {                   // if an obstacle, update map data and go to next loop iteration
                    intMap[c.clusterPos.x][c.clusterPos.y][xdx][ydy] = 1;
                    continue;
                }
                m0 = new PathTile(p, GP->map_hierarchy, n0->get_level(), n0->get_priority());
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
// Constructor
Path_Hierarchy::Path_Hierarchy(const int numClusters) : _transitionS{}, _clusterS{} {
    _numTrans = 0;
    _numClusters = numClusters;
}
// Destructor
Path_Hierarchy::~Path_Hierarchy() {
    PathTile* temp1, * temp2;
    for (int i = 0; i < _numTrans; i++) {
        getTransitionTileAddresses(i, temp1, temp2);
        delete temp1;
        delete temp2;
    }
}

//////////////

void Path_Hierarchy::addTransition(const std::pair<PathTile*, PathTile*>& tilePair) {
    _transitionS.push_back(std::pair<PathTile*, PathTile*> {tilePair.first, tilePair.second});
    _numTrans++;
}


void Path_Hierarchy::buildClusterS() {
    for(int j = 0; j < CLUSTER_YNUM; j++) {
        for(int i = 0; i < CLUSTER_XNUM; i++)
            _clusterS.push_back(Cluster{ {i,j}, {i*CLUSTER_TLENGTH, j*CLUSTER_TLENGTH}});
    }
}


// returns pair of adjacent tiles from cluster pos and tile displacement
std::pair<PathTile*, PathTile*> getAdjTiles(const Cluster& c1, const Cluster& c2, const int k, const bool adjOrientation) {
    Point p1, p2;
    if (adjOrientation) {   // Cluster 1 on top of Cluster 2
        p1.x = k + c1.tilePos.x; p1.y = c2.tilePos.y - 1;
        p2.x = k + c1.tilePos.x; p2.y = c2.tilePos.y;
        // From the Heap
        return std::make_pair(getPathTileFromPoint(p1), getPathTileFromPoint(p2));
    }
    else {                  // Cluster 1 on left of Cluster 2
        p1.x = c2.tilePos.x; p1.y = k + c2.tilePos.y;
        p2.x = c2.tilePos.x - 1; p2.y = k + c2.tilePos.y;
        // From the Heap
        return std::make_pair(getPathTileFromPoint(p1), getPathTileFromPoint(p2));
    }
}


// Accessors
//std::vector<std::pair<PathTile, PathTile>> Path_Hierarchy::get_transitionS() { return _transitionS; }
std::pair<PathTile, PathTile> Path_Hierarchy::get_transition(const int transNum) {
    return std::make_pair(*_transitionS[transNum].first, *_transitionS[transNum].second); }
std::vector<Cluster> Path_Hierarchy::get_clusterS() {return _clusterS;}
Cluster Path_Hierarchy::get_cluster(const Point p) {
    Cluster c = _clusterS[p.y * CLUSTER_XNUM + static_cast<int64_t>(p.x)];
        return _clusterS[p.y * CLUSTER_XNUM + static_cast<int64_t>(p.x)];
}
Cluster Path_Hierarchy::get_cluster(const int cNum) { return _clusterS[cNum]; }
int Path_Hierarchy::get_numTrans() { return _numTrans; }
int Path_Hierarchy::get_numClusters() { return _numClusters; }

// Private Memory Accessors

bool Path_Hierarchy::getTransitionTileAddresses(const int transNum, PathTile*& t1, PathTile*& t2) {
    if (transNum >= _numTrans)
        return false;
    t1 = _transitionS[transNum].first;
    t2 = _transitionS[transNum].second;
    return true;
}


//////////////////////////////////////////////////////////////////////////
// get cluster Point from Cluster Num
Point getClusterPFromNum(const int clusterNum) {
    int x = clusterNum % static_cast<int32_t>(CLUSTER_XNUM);
    int y = (clusterNum - x) / static_cast<int32_t>(CLUSTER_XNUM);
    return { x, y };
}
// gets cluster index in _clusterS vector
int getClusterNum(const Point& p) {
    return(p.y*static_cast<int32_t>(CLUSTER_XNUM) + p.x);
}

// Function abstractMap
// divides map into clusters and defines transition points between them
void abstractMap() {

    GP->map_hierarchy->buildClusterS();

    // Adding entrances between clusters for all adjacent clusters
    for(int i = 0; i < CLUSTER_XNUM; i++) {     // checking all vertically adjacent clusters
        for(int j =0; j < CLUSTER_YNUM -1; j++) {
            findTransitions(GP->map_hierarchy->get_cluster({ i,j }), GP->map_hierarchy->get_cluster({ i, j + 1 }));
        }
    }

    for(int i = 0; i < CLUSTER_XNUM - 1; i++) {     // checking all laterally adjacent clusters
        for(int j = 0; j < CLUSTER_YNUM; j++) {
            findTransitions(GP->map_hierarchy->get_cluster({ i,j }), GP->map_hierarchy->get_cluster({ i + 1, j }));
        }
    }

}

// Function findTransitions
// takes two adjacent clusters and finds transition points between them
void findTransitions(const Cluster& c1, const Cluster& c2) {
    Path_Hierarchy* p = GP->map_hierarchy;
    std::vector<std::vector<bool>> m = GP->_collisionM;
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
            validEnt = false;
            if (entStart == entEnd) {  // if single-tile entrance, define one transition point

                GP->map_hierarchy->addTransition(getAdjTiles(c1, c2, entStart, adjOrientation));
            }
            else if(entEnd - entStart < MIN_ENTRANCE_LENGTH) {  // if small entrance, define one transition point in middle

                GP->map_hierarchy->addTransition(getAdjTiles(c1, c2, entStart + static_cast<int>((entEnd-entStart)/2), adjOrientation));
            }
            else {      // otherwise define multiple transition points
                for(int k = entStart; k < entEnd+1; k += TRANSITION_INTERVAL)

                    GP->map_hierarchy->addTransition(getAdjTiles(c1, c2, k, adjOrientation));
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
    int numTrans = GP->map_hierarchy->get_numTrans();
    int numClusters = GP->map_hierarchy->get_numClusters();
    for (int i = 0; i < numTrans; i++) {
        if (!GP->map_hierarchy->getTransitionTileAddresses(i, t1, t2))
            break;
        cNum1 = getClusterNum(t1->getParentCluster().clusterPos);
        cNum2 = getClusterNum(t2->getParentCluster().clusterPos);
        v1 = Vertex(GP->map_graph->getVCNum(cNum1), cNum1, t1);
        v2 = Vertex(GP->map_graph->getVCNum(cNum2), cNum2, t2);
        GP->map_graph->addVertex(v1, cNum1);
        GP->map_graph->addVertex(v2, cNum2);
        if (v1.t->getParentCluster().clusterPos.x == v2.t->getParentCluster().clusterPos.x) {  // if v1 and v2 vertically adjacent
            if(v1.t->get_mapRelPos().y < v2.t->get_mapRelPos().y )
                GP->map_graph->addEdge(t1->get_mapRelPos(), t2->get_mapRelPos(), 1, INTER, { 2 });     // {0} is the path of going down once
            else
                GP->map_graph->addEdge(t1->get_mapRelPos(), t2->get_mapRelPos(), 1, INTER, { 6 });     // {0} is the path of going up once
        }
        else { // if v1 and v2 horizontally adjacent
            if(v1.t->get_mapRelPos().x < v2.t->get_mapRelPos().x)
                GP->map_graph->addEdge(t1->get_mapRelPos(), t2->get_mapRelPos(), 1, INTER, { 0 });     // {2} is the path of going right once
            else
                GP->map_graph->addEdge(t1->get_mapRelPos(), t2->get_mapRelPos(), 1, INTER, { 4 });     // {2} is the path of going left once
        }
    }
    int numVC;
    double distance;
    std::vector<int> path;
    for (int cNum = 0; cNum < numClusters; cNum++) {
        numVC = GP->map_graph->getVCNum(cNum);
        for (int i = 0; i < numVC; i++) {
            for (int j = 0; j < numVC; j++) {
                if (i == j)
                    continue;
                if (!(GP->map_graph->getVertexCopy(i, cNum, v1) && GP->map_graph->getVertexCopy(j, cNum, v2)))
                    break;
                path = pathFind(v1.t->get_clusterRelPos(), v2.t->get_clusterRelPos(), cNum);
                distance = pathToDistance(path);
                if (distance != HUGE_VAL)
                    GP->map_graph->addEdge(v1.t->get_mapRelPos(), v2.t->get_mapRelPos(), distance, INTRA, path);
            }
        }
    }
}

void buildGraphPaths() {

    int V = GP->map_graph->getVNum();   // get number of vertices

    GP->map_graph->setWeightedAdj();   // encapsulates weightedAdj  inside abstract_graph
    GP->map_graph->setNeighborSet();



    std::vector<std::vector<double>> distances;
    std::vector<double>* min_distance = new std::vector<double>(V);
    std::vector<int> previous(V);


    std::vector<std::vector<std::vector<int>*>*>* paths = new std::vector<std::vector<std::vector<int>*>*>(V);
    std::vector<std::vector<int>*>* pathsFromSrc;

    for (int i = 0; i < V; i++) {
        pathsFromSrc = new std::vector<std::vector<int>*>;
        DijkstraComputePaths(i, GP->map_graph->get_neighborSet(), min_distance, previous);
        for (int j = 0; j < V; j++) {
            pathsFromSrc->push_back(DijkstraGetShortestPathTo(j, previous));    // gets from heap
        }
        paths->at(i) = pathsFromSrc;
        distances.push_back(*min_distance);
    }
    GP->map_graph->setPaths(paths);
    GP->map_graph->setDistances(distances);

    // cleanup
    pathsFromSrc = nullptr;
    paths = nullptr;
    delete min_distance;
    min_distance = nullptr;


}



// RUNTIME ALGO

std::vector<int> searchForPath(const Point& startP, const Point& goalP) {
    Vertex startV, * startVP, goalV, * goalVP, transV, v;
    Edge tempEdge;
    PathTile* tempT;
    bool addStart(false), addGoal(false);
    Point p1, p2;
    std::vector<int> intPath;
    std::vector<Vertex*> graphPath;
    int cNum1 = getClusterNum(findParentCluster(startP).clusterPos), cNum2 = getClusterNum(findParentCluster(goalP).clusterPos);
    /*
    if start Cluster == goal Cluster
        if both in graph
            return path
        A star algorithm
        if soln
            return path
    */
    if (cNum1 == cNum2) {            // start and goal in same cluster
        if (GP->map_graph->getVertexCopy(startP, startV) && GP->map_graph->getVertexCopy(goalP, goalV)) {     // start and goal vertices already in graph
            if (GP->map_graph->getEdge(startV, goalV, tempEdge))
                return tempEdge.path;
        }
        intPath = pathFind({ startP.x % static_cast<int32_t>(CLUSTER_TLENGTH), startP.y % static_cast<int32_t>(CLUSTER_TLENGTH) },
            { goalP.x % static_cast<int32_t>(CLUSTER_TLENGTH), goalP.y % static_cast<int32_t>(CLUSTER_TLENGTH) }, cNum1);
        if (intPath.size() > 0)
            return intPath;
    }


    int startTransitionNum = GP->map_graph->getVCNum(cNum1);
    int goalTransitionNum = GP->map_graph->getVCNum(cNum2);
    std::vector<double> startDistances;
    std::vector<std::vector<int>> startToTransPaths;
    std::vector<double> goalDistances;
    std::vector<std::vector<int>> goalToTransPaths;

    // Connecting start and goal to cluster border transitions, and finding shortest path index
    if (!GP->map_graph->getVertexCopy(startP, startV)) {
        Point sP = { startP.x % static_cast<int32_t>(CLUSTER_TLENGTH), startP.y % static_cast<int32_t>(CLUSTER_TLENGTH) };
        for (int i = 0; i < startTransitionNum; i++) {
            if (!GP->map_graph->getVertexCopy(i, cNum1, transV)) {
                startToTransPaths.push_back({});
                startDistances.push_back(max_weight);
                continue;
            }
            startToTransPaths.push_back(pathFind(sP, transV.t->get_clusterRelPos(), cNum1));
            startDistances.push_back(pathToDistance(startToTransPaths[i]));
        }

        addStart = true;

    }

    if (!GP->map_graph->getVertexCopy(goalP, goalV)) {
        Point gP = { goalP.x % static_cast<int32_t>(CLUSTER_TLENGTH), goalP.y % static_cast<int32_t>(CLUSTER_TLENGTH) };
        for (int i = 0; i < goalTransitionNum; i++) {
            if (!GP->map_graph->getVertexCopy(i, cNum2, transV)) {
                goalToTransPaths.push_back({});
                goalDistances.push_back(max_weight);
                continue;
            }
            goalToTransPaths.push_back(pathFind(gP, transV.t->get_clusterRelPos(), cNum2));
            goalDistances.push_back(pathToDistance(goalToTransPaths[i]));
        }

        addGoal = true;

    }



    int minX = 0, minY = 0, minI = 0, minJ = 0, x, y;
    double temp = max_weight, transDist;

    // Getting and building shortest path into direction vector
    if (addStart && addGoal) {
        for (int i = 0; i < startTransitionNum; i++) {
            if (startDistances[i] == max_weight) continue;
            for (int j = 0; j < goalTransitionNum; j++) {
                if (goalDistances[j] == max_weight) continue;

                x = GP->map_graph->keyToGlobalK(i, cNum1);
                y = GP->map_graph->keyToGlobalK(j, cNum2);
                if (GP->map_graph->getDistance(x, y) == max_weight) continue;

                transDist = startDistances[i] + goalDistances[j] + GP->map_graph->getDistance(x, y);
                if (transDist < temp) {
                    temp = transDist;
                    minI = i; minJ = j;
                    minX = x; minY = y;
                }
            }
        }

        intPath = GP->map_graph->getIntPathFromGraph(minX, minY);
        std::vector<int> goalPath = reverseIntPath(goalToTransPaths[minJ]);
        std::vector<int> startPath = startToTransPaths[minI];
        intPath.insert(intPath.begin(), startPath.begin(), startPath.end());
        intPath.insert(intPath.end(), goalPath.begin(), goalPath.end());
    }
    else if (addStart) {
        minY = GP->map_graph->keyToGlobalK(goalV.key, cNum2);
        for (int i = 0; i < startTransitionNum; i++) {
            if (startDistances[i] == max_weight) continue;

            x = GP->map_graph->keyToGlobalK(i, cNum1);
            if (GP->map_graph->getDistance(x, minY) == max_weight) continue;

            transDist = startDistances[i] + GP->map_graph->getDistance(x, minY);
            if (transDist < temp) {
                temp = transDist;
                minI = i;  minX = x;
            }
        }
        intPath = GP->map_graph->getIntPathFromGraph(minX, minY);
        std::vector<int> startPath = startToTransPaths[minI];
        intPath.insert(intPath.begin(), startPath.begin(), startPath.end());

    }
    else if (addGoal) {
        minX = GP->map_graph->keyToGlobalK(startV.key, cNum1);
        for (int i = 0; i < goalTransitionNum; i++) {
            if (goalDistances[i] == max_weight) continue;

            y = GP->map_graph->keyToGlobalK(i, cNum2);
            if (GP->map_graph->getDistance(minX, y) == max_weight) continue;

            transDist = goalDistances[i] + GP->map_graph->getDistance(minX, y);
            if (transDist < temp) {
                temp = transDist;
                minY = y; minJ = i;
            }
        }

        intPath = GP->map_graph->getIntPathFromGraph(minX, minY);
        std::vector<int> goalPath = reverseIntPath(goalToTransPaths[minJ]);
        intPath.insert(intPath.end(), goalPath.begin(), goalPath.end());

    }
    else {
        minX = GP->map_graph->keyToGlobalK(startV.key, cNum1);
        minY = GP->map_graph->keyToGlobalK(goalV.key, cNum2);
        intPath = GP->map_graph->getIntPathFromGraph(minX, minY);
    }


    //printMAP();
    return intPath;

}

// VERY IMPORTANT FUNCTION
// translates sprite pixel position (which is typically top left corner) to tile position
// can change values here if we want middle of sprite to be center of collision, or feet!
Point playerToTPos(const Point& p, const Point& size, const float scale) {
    double x = (static_cast<double>(p.x)+size.x * scale / 2) / static_cast<double>(TLENGTH);
    double y = (static_cast<double>(p.y)+ size.y * scale * 0.85) / static_cast<double>(TLENGTH);
    return { static_cast<int>(floor(x)), static_cast<int>(floor(y)) };
}

Point zombieToTPos(const Point& p, const Point& size, const float scale) {
    double x = (static_cast<double>(p.x) + size.x * scale / 2) / static_cast<double>(TLENGTH);
    double y = (static_cast<double>(p.y) + size.y * scale*0.95) / static_cast<double>(TLENGTH);
    return { static_cast<int>(floor(x)), static_cast<int>(floor(y)) };
}


// Called by constructor, finds parent cluster from tile point
Cluster findParentCluster(const Point& mapRelPos) {
    Point p = { mapRelPos.x / static_cast<int32_t>(CLUSTER_TLENGTH) , mapRelPos.y / static_cast<int32_t>(CLUSTER_TLENGTH) };
    return GP->map_hierarchy->get_cluster(p);
}

void resetIntMap() {
    //static int intMap[CLUSTER_XNUM][CLUSTER_YNUM][CLUSTER_TLENGTH][CLUSTER_TLENGTH]
    for (int i = 0; i < CLUSTER_XNUM; i++) {
        for (int j = 0; j < CLUSTER_YNUM; j++) {
            for (int k = 0; k < CLUSTER_TLENGTH; k++) {
                for (int l = 0; l < CLUSTER_TLENGTH; l++) {
                    intMap[i][j][k][l] = -1;
                }
            }
        }
    }
}


void pathPreprocessing(const int numClusters, const Map& map) {
    //need to reset GP and static intMap
    GP->reset(numClusters, map);
    resetIntMap();
    Log::verbose("Path variables initialized");
    abstractMap();
    Log::verbose("Map abstracted");
    buildGraph();
    Log::verbose("Graph built");
    buildGraphPaths();
    Log::verbose("Graph Paths built");
    //printMAP();
}

void printMAP() {
    int i, j;
    cout << "Map\n";
    cout << "\n  ";
    for (j = 0; j < CLUSTER_TLENGTH * CLUSTER_XNUM; j++)
        if (j < 10)
            cout << " " << j << " ";
        else
            cout << j << " ";
    cout << endl;
    for (j = 0; j < CLUSTER_TLENGTH * CLUSTER_YNUM; j++) {
        if (j < 10)
            cout << " " << j << " ";
        else
            cout << j << " ";
        for (i = 0; i < CLUSTER_TLENGTH * CLUSTER_XNUM; i++) {
            if (GP->_collisionM[i][j]) cout << "-" << "  ";
            else cout << "O" << "  ";
        }
        cout << endl;
    }
    cout << endl;
}


