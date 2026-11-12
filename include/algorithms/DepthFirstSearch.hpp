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

    static std::vector<int> findPath(
        const Graph& graph,
        int source,
        int destination
    );
};
