#pragma once

#include <vector>

#include "graph/Graph.hpp"

class AdjacencyListGraph : public Graph
{
public:
    AdjacencyListGraph(bool directed, bool weighted)
        : Graph(directed, weighted)
    {
    }

    bool addVertex() override;
    bool removeVertex(int index) override;

    void printGraph() const override;

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

    std::vector<int> getNeighbors(
        int vertex
    ) const override;

private:
    struct Edge
    {
        int destination;
        float weight;
    };

    struct Vertex
    {
        std::vector<Edge> connections;
    };

    std::vector<Vertex> vertexList_;
};