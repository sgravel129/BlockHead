#include "pathRes.hpp"

///////////////////
// Global Path Vars
GlobalPathVars::GlobalPathVars() : map_hierarchy(nullptr), map_graph(nullptr), _collisionM({}) {}
GlobalPathVars::~GlobalPathVars() {
	delete map_hierarchy;
	delete map_graph;
}
void GlobalPathVars::reset(const int numClusters, const Map& map) {
	delete map_hierarchy;
	delete map_graph;
	map_hierarchy = new Path_Hierarchy(numClusters);
	map_graph = new Abstract_Graph(numClusters);
	
	std::vector<std::vector<bool>> col = map.getColM();
	std::vector<bool> temp;
	for (int j = 0; j < col[0].size(); j++) {
		for (int i = 0; i < col.size(); i++) {
			temp.push_back(!col[i][j]);
		}
		_collisionM.push_back(temp);
		temp.clear();
	}
	

}


void GlobalPathVars::setGlobals(Path_Hierarchy* const mp, Abstract_Graph* const mg, const std::vector<std::vector<bool>>& colM) { 
	map_hierarchy = mp; map_graph = mg; _collisionM = colM;
}













