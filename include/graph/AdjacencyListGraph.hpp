#pragma once

#include <vector>

#include "graph/Graph.hpp"

class AdjacencyListGraph : public Graph
{
public:
    AdjacencyListGraph(bool directed, bool weight)
        : Graph(directed, weight)
    {
    }

    bool addVertex() override;
    bool removeVertex() override;

    bool addEdge(
        int source,
        int destination,
        float weight = 1.0f
    ) override;

    bool removeEdge(
        int source,
        int destination
    ) override;

    bool hasEdge(
        int source,
        int destination
    ) const override;

    float getEdgeWeight(
        int source,
        int destination
    ) const override;

private:
    struct Vertex
    {   
        int index
        std::vector<int> conections;
    }

    std::vector<Vertex> vertexList_
} 