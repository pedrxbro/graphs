#include "graph/AdjacencyMatrixGraph.hpp"

bool AdjacencyMatrixGraph::addVertex()
{
    // Calcula o novo tamanho da matriz.
    const std::size_t newSize = adjacencyMatrix_.size() + 1;

    // Aumenta cada linha adicionando uma coluna com valor 0.
    for (auto& row : adjacencyMatrix_)
    {
        row.resize(newSize, 0.0f);
    }

    // Adiciona uma nova linha para o novo vértice.
    adjacencyMatrix_.emplace_back(newSize, 0.0f);

    return true;
}

bool AdjacencyMatrixGraph::removeVertex(int index)
{

    // Verifica se o índice do vértice é válido.
    if (index < 0 || index >= static_cast<int>(adjacencyMatrix_.size()))
    {
        return false;
    }

    // Remove a linha
    adjacencyMatrix_.erase(adjacencyMatrix_.begin() + index);


    // Remove a coluna
    for (auto& row : adjacencyMatrix_)
    {
        row.erase(row.begin() + index);
    }

    return true;
}