#include "testPath.hpp"

using namespace std;

#define X_MAX 1200
#define Y_MAX 1200
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



Path_Hierarchy* dummyPathHierarchy() {
	Path_Hierarchy* ph = new Path_Hierarchy(CLUSTER_XNUM * CLUSTER_YNUM);
	ph->buildClusterS();


	return ph;

}


Abstract_Graph* dummyGraph() {
	return new Abstract_Graph(CLUSTER_XNUM * CLUSTER_YNUM);
}







void testGraph() {

}



void printGraph() {

}

void printPathHierarchy() {

}

void printTile(PathTile* const t) {
	cout << "PathTile Info:\n" << endl;
	cout << "Map relative coordinates:\t" << t->get_mapRelPos().x << ", " << t->get_mapRelPos().y << endl;
	cout << "Cluster relative coordinates:\t" << t->get_clusterRelPos().x << ", " << t->get_clusterRelPos().y << endl;
	cout << "Cluster......clusterPos:  " << t->getParentCluster().clusterPos.x << ", " << t->getParentCluster().clusterPos.y
		<< "\ttilePos:  " << t->getParentCluster().tilePos.x << ", " << t->getParentCluster().tilePos.y << endl;
	cout << "Traversable:\t" << t->get_traversable() << endl;
	cout << "Level:\t" << t->get_level() << "\tPriority:\t" << t->get_priority() << endl << endl;
}

void testTile() {
	GP.setGlobals(dummyPathHierarchy(), dummyGraph());
	PathTile* tile = new PathTile({ 50,100 }, true, GP.map_hierarchy);
	printTile(tile);

}
/*
int main() {
	testTile();
	return 0;
}
*/