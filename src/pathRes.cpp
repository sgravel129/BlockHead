#include "pathRes.hpp"

///////////////////
// Global Path Vars
GlobalPathVars::GlobalPathVars() : map_hierarchy{}, map_graph{} {}
void GlobalPathVars::setGlobals(Path_Hierarchy* const mp, Abstract_Graph* const mg) { map_hierarchy = mp; map_graph = mg; }














