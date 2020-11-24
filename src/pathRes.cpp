#include "pathRes.hpp"

///////////////////
// Global Path Vars
GlobalPathVars::GlobalPathVars() : map_hierarchy{}, map_graph{} {}
void GlobalPathVars::setGlobals(Path_Hierarchy* const mp, Abstract_Graph* const mg, const std::vector<std::vector<bool>>& colM) { 
	map_hierarchy = mp; map_graph = mg; _collisionM = colM;
}














