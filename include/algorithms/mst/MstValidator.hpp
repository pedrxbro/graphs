#pragma once

#include "graph/Graph.hpp"

class MstValidator
{
public:
    // True if every edge (u, v) also exists as (v, u).
    // Graph has no public "directed" getter, so we check symmetry.
    static bool isUndirected(const Graph& graph);
};
