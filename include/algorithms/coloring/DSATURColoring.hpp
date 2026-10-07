#pragma once

#include "algorithms/coloring/ColoringResult.hpp"
#include "graph/Graph.hpp"

class DSATURColoring
{
public:
    static ColoringResult execute(
        const Graph& graph
    );
};