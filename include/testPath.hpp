#include "dijkstra.hpp"
#include "graph.hpp"
#include "path.hpp"
#include "pathRes.hpp"


Path_Hierarchy* dummyPathHierarchy();
Abstract_Graph* dummyGraph();
void testGraph();
void printGraph();
void printPathHierarchy();
void printTile(PathTile* const t);
void testTile();
void testMain();
void testHierarchy();
void testAstar(const Point&, const Point&, const int);
void printMap(const Point&);
void testDijkstra();