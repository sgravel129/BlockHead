#pragma once

#include "graph.hpp"



std::vector<int> searchForPath(const Point&, const Point&);
std::pair<int*, int*> Dijkstra(const int, const int, int**);

int minDistance(int dist[], bool sptSet[], const int);

std::vector<int> graphPathToIntPath(const std::vector<Vertex*>&);
