#pragma once

#include "algorithms/coloring/ColoringResult.hpp"
#include "graph/Graph.hpp"

class WelshPowellColoring
{
public:
    static ColoringResult execute(
        const Graph& graph
    );
};