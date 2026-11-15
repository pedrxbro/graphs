#pragma once

#include "graph/Graph.hpp"

struct LocalSearchResult
{
    double initialFlow;
    double finalFlow;
    int steps;
};

class LocalSearchFlow
{
public:
    static LocalSearchResult execute(
        const Graph& graph,
        int source,
        int sink
    );
};