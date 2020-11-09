
#include <vector>
#include "constants.hpp"

#define X_MAX SCREEN_WIDTH
#define Y_MAX SCREEN_HEIGHT
// will change these depending on amount of pixels sprite occupies
#define X_STEP 30
#define Y_STEP 30

struct Node
{

    int x;
    int y;
    bool nonObst; // True if zombie/player can walk across node, False if obstacle
};

void abstractMap();
void buildGraph();
void addLevelToGraph(int l);
void preprocessing(int maxLevel);
