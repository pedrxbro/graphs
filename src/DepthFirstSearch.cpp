#include "algorithms/DepthFirstSearch.hpp"

#include <vector>

namespace
{
    void visitVertex(
        const Graph &graph,
        int currentVertex,
        std::vector<bool> &visited,
        std::vector<int> &visitOrder)
    {
        visited[currentVertex] = true;
        visitOrder.push_back(currentVertex);

        for (int neighbor : graph.getNeighbors(currentVertex))
        {
            if (!visited[neighbor])
            {
                visitVertex(
                    graph,
                    neighbor,
                    visited,
                    visitOrder);
            }
        }
    }
}

std::vector<int> DepthFirstSearch::execute(
    const Graph &graph,
    int source)
{
    std::vector<int> visitOrder;

    int vertexCount = graph.getVertexCount();

    if (source < 0 || source >= vertexCount)
    {
        return visitOrder;
    }

    std::vector<bool> visited(vertexCount, false);

    visitVertex(
        graph,
        source,
        visited,
        visitOrder);

    return visitOrder;
}
