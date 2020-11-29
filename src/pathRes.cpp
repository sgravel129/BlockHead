#include "pathRes.hpp"

///////////////////
// Global Path Vars
GlobalPathVars::GlobalPathVars() : map_hierarchy(nullptr), map_graph(nullptr) {}
GlobalPathVars::~GlobalPathVars() {
	delete map_hierarchy;
	delete map_graph;
}
void GlobalPathVars::reset(const int numClusters) {
	delete map_hierarchy;
	delete map_graph;
	map_hierarchy = new Path_Hierarchy(numClusters);
	map_graph = new Abstract_Graph(numClusters);
}


void GlobalPathVars::setGlobals(Path_Hierarchy* const mp, Abstract_Graph* const mg, const std::vector<std::vector<bool>>& colM) { 
	map_hierarchy = mp; map_graph = mg; _collisionM = colM;
}













