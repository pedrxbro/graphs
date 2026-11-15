#pragma once

#include "graph/Graph.hpp"

class FordFulkerson
{
public:
    static double execute(
        const Graph& graph,
        int source,
        int sink
    );
};
