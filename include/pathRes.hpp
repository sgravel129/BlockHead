
#include "path.hpp"
#include "graph.hpp"

class GlobalPathVars {
public:
    GlobalPathVars();
    ~GlobalPathVars();
    void reset(const int);
    void setGlobals(Path_Hierarchy* const, Abstract_Graph* const, const std::vector<std::vector<bool>>&);
public:
    Path_Hierarchy* map_hierarchy;
    Abstract_Graph* map_graph;

    std::vector<std::vector<bool>> _collisionM;
};

extern GlobalPathVars* GP;

