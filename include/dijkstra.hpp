#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <list>

#include <limits> // for numeric_limits

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>

#include "graph.hpp"



std::vector<int> searchForPath(const Point&, const Point&);


std::vector<int> graphPathToIntPath(const std::vector<Vertex*>&);



void DijkstraComputePaths(vertex_t source,
    std::vector<std::vector<neighbor>*>* const adjacency_list,
    std::vector<weight_t>* const min_distance,
    std::vector<vertex_t>& previous);

std::vector<vertex_t>* DijkstraGetShortestPathTo(
    vertex_t vertex, const std::vector<vertex_t>& previous);