#include "graph/AdjacencyMatrixGraph.hpp"

bool AdjacencyMatrixGraph::addVertex()
{
    const std::size_t newSize = adjacencyMatrix_.size() + 1;

    for (auto& row : adjacencyMatrix_)
    {
        row.resize(newSize, 0.0f);
    }

    adjacencyMatrix_.emplace_back(newSize, 0.0f);

    return true;
}