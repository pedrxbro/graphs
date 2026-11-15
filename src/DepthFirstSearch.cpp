
#include "algorithms/DepthFirstSearch.hpp"

#include <vector>

namespace
{
    bool visitVertex(
        const Graph& graph,
        int currentVertex,
        std::vector<bool>& visited,
        std::vector<int>& result,
        bool searchingPath,
        int destination
    )
    {
        visited[currentVertex] = true;
        result.push_back(currentVertex);

        if (searchingPath && currentVertex == destination)
        {
            return true;
        }

        for (int neighbor : graph.getNeighbors(currentVertex))
        {
            if (searchingPath &&
                graph.getEdgeWeight(
                    currentVertex,
                    neighbor
                ) <= 0.0f)
            {
                continue;
            }

            if (!visited[neighbor])
            {
                if (visitVertex(
                    graph,
                    neighbor,
                    visited,
                    result,
                    searchingPath,
                    destination
                ))
                {
                    return true;
                }
            }
        }

        if (searchingPath)
        {
            result.pop_back();
        }

        return false;
    }
}

std::vector<int> DepthFirstSearch::execute(
    const Graph& graph,
    int source
)
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
        visitOrder,
        false,
        -1
    );

    return visitOrder;
}

std::vector<int> DepthFirstSearch::findPath(
    const Graph& graph,
    int source,
    int destination
)
{
    std::vector<int> path;

    const int vertexCount = graph.getVertexCount();

    if (source < 0 ||
        destination < 0 ||
        source >= vertexCount ||
        destination >= vertexCount)
    {
        return path;
    }

    std::vector<bool> visited(vertexCount, false);

    visitVertex(
        graph,
        source,
        visited,
        path,
        true,
        destination
    );

    return path;
}