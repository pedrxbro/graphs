#pragma once

#include "algorithms/mst/MstResult.hpp"
#include "graph/Graph.hpp"

class PrimMst
{
public:
    // Undirected graphs only. O(E log V).
    static MstResult execute(const Graph& graph);
};
