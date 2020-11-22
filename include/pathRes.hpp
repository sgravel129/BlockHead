
#include "path.hpp"
#include "graph.hpp"

class GlobalPathVars {
public:
    GlobalPathVars();
    void setGlobals(Path_Hierarchy* const, Abstract_Graph* const);
public:
    Path_Hierarchy* map_hierarchy;
    Abstract_Graph* map_graph;
};

extern GlobalPathVars GP;