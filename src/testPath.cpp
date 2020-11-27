#include "testPath.hpp"
#include <ctime>

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

std::vector<std::vector<bool>> dummyMap() {
	std::vector<std::vector<bool>> m(X_MAX / X_STEP);
	int randInt;
	for (int i = 0; i < X_MAX / X_STEP; i++) {
		for (int j = 0; j < Y_MAX / Y_STEP; j++) {
			
			randInt = rand() % 100;
			if (randInt < 80)  m[i].push_back(true);
			else m[i].push_back(false);
			
			//m[i].push_back(true);
		}
	}
	return m;
}




void printMap(const Point& cPoint) {
	int i, j;
	cout << "Map of Cluster (" << cPoint.x << ", " << cPoint.y << ")\n";
	cout << "\n  ";
	for (j = 0; j < CLUSTER_TLENGTH; j++)
		if (j < 10)
			cout << " " << j << " ";
		else
			cout << j << " ";
	cout << endl;
	for (j = 0; j < CLUSTER_TLENGTH; j++) {
		if (j < 10)
			cout << " " << j << " ";
		else
			cout << j << " ";
		for (i = 0; i < CLUSTER_TLENGTH; i++) {
			if (GP._collisionM[(cPoint.x*CLUSTER_TLENGTH)+i][(cPoint.y * CLUSTER_TLENGTH)+j]) cout << "-" << "  ";
			else cout << "O" << "  ";
		}
		cout << endl;
	}
	cout << endl;
} // end showJumble



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

void testAstar(const Point& p1, const Point& p2, const int cNum) {
	std::vector<int> path = pathFind(p1, p2, cNum);
	cout << "Path from (" << p1.x << ", " << p1.y << ") to (" << p2.x << ", " << p2.y << ") in Cluster " << cNum << std::endl;
	for (int i = 0; i < path.size(); i++)
		cout << path[i];

}

void testGraph() {
	cout << "Checking a random edge in our graph..." << endl;
	int cNum = rand() % static_cast<__int32>(CLUSTER_XNUM * CLUSTER_YNUM);
	int vCNum = GP.map_graph->getVCNum(cNum);
	Vertex v1, v2;
	Edge e;
	do {
		while (!GP.map_graph->getVertexCopy(rand() % vCNum, cNum, v1));
		while (!GP.map_graph->getVertexCopy(rand() % vCNum, cNum, v2));
	} while (!GP.map_graph->getEdge(v1, v2, e));

	printMap(getClusterPFromNum(cNum));
	testAstar(v1.t->get_clusterRelPos(), v2.t->get_clusterRelPos(), cNum);
	cout << endl << "Should be same path as here:\t";
	for (int i = 0; i < e.path.size(); i++)
		cout << e.path[i];
	cout << endl << "distance: " << e.distance << endl << endl;

}


void testDijkstra() {
	int V = GP.map_graph->getVNum();   // get number of vertices
	GP.map_graph->setWeightedAdj();
	GP.map_graph->setNeighborSet();     // gets from heap
	std::vector<double>* min_distance = new std::vector<double>(V);
	std::vector<int> previous(V);
	int src = 0;
	DijkstraComputePaths(src, GP.map_graph->get_neighborSet(), min_distance, previous);
	
	std::vector<std::vector<int>*> paths;
	for(int i = 0; i < V; i++) paths.push_back(DijkstraGetShortestPathTo(i, previous));

	cout << "Path distances from src = " << src << ":\n";
	for (int i = 0; i < min_distance->size(); i++) cout << i << ":\t" << min_distance->at(i) << endl;
	cout << endl << "Paths from src = " << src << ":\n";
	for (int i = 0; i < V; i++) {
		cout << src << " to " << i << ":\t";
		for (int j = 0; j < paths[i]->size(); j++) {
			cout << paths[i]->at(j) << " ";
		}
		cout << endl;
	}

}


void testMain() {
	clock_t begin, end;
	
	srand(time(NULL));
	GP.setGlobals(dummyPathHierarchy(), dummyGraph(), dummyMap());
	GlobalPathVars lmao = GP;
	abstractMap();
	buildGraph();
	buildGraphPaths();

	// testing A star aglo
	//Point p1 = { 0,0 }, p2 = { 7,7 };
	//Point clusterP = { 3, 2 };
	//printMap(clusterP);
	//testAstar(p1, p2, getClusterNum(clusterP));
	//cout << endl << endl;
	
	//testTile();
	
	//testGraph();
	//begin = clock();
	//testDijkstra();
	//end = clock();
	//cout << endl << "Time elapsed: " << (end-begin) << endl;



	system("Pause");
	testHierarchy();
}



/*
int main() {
	testTile();
	return 0;
}
*/