#pragma once

#include <string>
#include <vector>

class Graph
{
public:
    // Cria um grafo.
    // Direcionado (directed) | Ponderado (weighted)
    Graph(bool directed, bool weighted)
        : directed_(directed),
        weighted_(weighted)
    {
    }

    virtual ~Graph() = default;

    virtual bool addVertex() = 0;

    virtual bool removeVertex(int index) = 0;
    
    virtual int getVertexCount() const = 0;

    virtual void printGraph() const = 0;

    virtual bool addEdge(
        int source,
        int destination,
        float weight = 1.0f
    ) = 0;

    virtual bool removeEdge(
        int source,
        int destination
    ) = 0;

    virtual bool hasEdge(
        int source,
        int destination
    ) const = 0;

    virtual float getEdgeWeight(
        int source,
        int destination
    ) const = 0;

    virtual std::vector<int> getNeighbors(
        int vertex
    ) const = 0;

protected:
    bool directed_;
    bool weighted_;
};