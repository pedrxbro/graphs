#pragma once

#include <vector>

struct MstEdge
{
    int source;
    int destination;
    float weight;
};

struct MstResult
{
    std::vector<MstEdge> edges;

    // Sum of the chosen edge weights
    double totalWeight = 0.0;

    // false = graph is disconnected, result is a spanning forest
    bool connected = true;
};
