#pragma once

#include "algorithms/mst/MstResult.hpp"
#include "graph/Graph.hpp"

class KruskalMst
{
public:
    // Undirected graphs only. O(E log E).
    static MstResult execute(const Graph& graph);
};
