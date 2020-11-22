#include "testPath.hpp"

using namespace std;

#define X_MAX 2000
#define Y_MAX 1200

#define X_STEP 50   // x size of tile
#define Y_STEP 50   // y size of tile

// will change these depending on amount of pixels sprite occupies
// #define CLUSTER_WIDTH 120
// #define CLUSTER_HEIGHT 120
#define CLUSTER_SLENGTH 400     // side length of cluster (square)
#define CLUSTER_TLENGTH CLUSTER_SLENGTH / X_STEP
#define CLUSTER_XNUM X_MAX/CLUSTER_SLENGTH
#define CLUSTER_YNUM Y_MAX/CLUSTER_SLENGTH


#define MIN_ENTRANCE_LENGTH 3   // minimum tile lenght for entrance
#define TRANSITION_INTERVAL 3   // number of tiles between transitions in single entrance



Path_Hierarchy* dummyPathHierarchy() {
	Path_Hierarchy* ph = new Path_Hierarchy(CLUSTER_XNUM * CLUSTER_YNUM);


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
	Path_Hierarchy temp = *GP.map_hierarchy;
	cout << "================\nPath Hierarchy Info:\n=========\n" << endl;
	cout << "Clusters:\n";
	for (int i = 0; i < CLUSTER_XNUM; i++) {
		for (int j = 0; j < CLUSTER_YNUM; j++) {
			Cluster tempC = temp.get_cluster({ i, j });
			cout << "Map:  " << tempC.clusterPos.x << ", " << tempC.clusterPos.y << "\tTile:  " << tempC.tilePos.x << ", " << tempC.tilePos.y << endl;
		}
	}
	/*
	for (int i = 0; i < GP.map_hierarchy->get_numTrans(); i++) {
		printTile(temp.get_transition(i).first);
		printTile(temp.get_transition(i).second);

	}
	*/
}

void printTile(const PathTile& t) {
	cout << "PathTile Info:\n" << endl;
	cout << "Map relative coordinates:\t" << t.get_mapRelPos().x << ", " << t.get_mapRelPos().y << endl;
	cout << "Cluster relative coordinates:\t" << t.get_clusterRelPos().x << ", " << t.get_clusterRelPos().y << endl;
	cout << "Cluster......clusterPos:  " << t.getParentCluster().clusterPos.x << ", " << t.getParentCluster().clusterPos.y
		<< "\ttilePos:  " << t.getParentCluster().tilePos.x << ", " << t.getParentCluster().tilePos.y << endl;
	cout << "Traversable:\t" << t.get_traversable() << endl;
	cout << "Level:\t" << t.get_level() << "\tPriority:\t" << t.get_priority() << endl << endl;
}

void testTile() {
	PathTile* tile = new PathTile({ 20,9 }, true, GP.map_hierarchy);
	printTile(*tile);

}

void testHierarchy() {
	printPathHierarchy();
}


void testMain() {
	GP.setGlobals(dummyPathHierarchy(), dummyGraph());
	abstractMap();
	//testTile();
	testHierarchy();
}



/*
int main() {
	testTile();
	return 0;
}
*/