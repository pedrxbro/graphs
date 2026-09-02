#pragma once

#include <vector>

#include "graph/Graph.hpp"

class DepthFirstSearch
{
public:
    static std::vector<int> execute(
        const Graph& graph,
        int source
    );
};
