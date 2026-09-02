#pragma once

#include <vector>

#include "graph/Graph.hpp"

struct DijkstraResult
{
    std::vector<float> distances;
    std::vector<int> previous;
};

class Dijkstra
{
public:
    static DijkstraResult execute(
        const Graph& graph,
        int source
    );

    static std::vector<int> buildPath(
        const DijkstraResult& result,
        int source,
        int destination
    );
};