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

bool AdjacencyMatrixGraph::addEdge(int source, int destination, float weight) 
{   
	// Verifica se os índices dos vértices são válidos.
    if (source < 0 ||
        destination < 0 ||
        source >= static_cast<int>(adjacencyMatrix_.size()) ||
        destination >= static_cast<int>(adjacencyMatrix_.size()))
    {
        return false;
    }

    const float edgeWeight = weighted_ ? weight : 1.0f;

	// Adiciona a aresta na matriz de adjacência.
	adjacencyMatrix_[source][destination] = edgeWeight;

    // Adicionando a ligação de volta
	if (!directed_)
	{
		adjacencyMatrix_[destination][source] = edgeWeight;
	}

    return true;
}

bool AdjacencyMatrixGraph::removeEdge(int source, int destination)
{
	// Verifica se são válidos os índices dos vértices.
    if (source < 0 ||
		destination < 0 ||
		source >= static_cast<int>(adjacencyMatrix_.size()) ||
        destination >=  static_cast<int>(adjacencyMatrix_.size()))
    { 
        return false;
    }

	if (adjacencyMatrix_[source][destination] == 0.0f)
	{
		return false; // A aresta não existe
	}

    // Remove a aresta da matriz de adjacência.
    adjacencyMatrix_[source][destination] = 0.0f;


    if (!directed_) {
		adjacencyMatrix_[destination][source] = 0.0f; // Remove a ligação de volta
    }

    return true;
}