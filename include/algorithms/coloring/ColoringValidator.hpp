#pragma once

#include <vector>

#include "graph/Graph.hpp"

class ColoringValidator
{
public:
    static bool isValid(
        const Graph& graph,
        const std::vector<int>& colors
    );
};