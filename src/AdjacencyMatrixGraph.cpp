#include "graph/AdjacencyMatrixGraph.hpp"

#include <iostream>

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

void AdjacencyMatrixGraph::printGraph() const
{
    if (adjacencyMatrix_.empty())
    {
        std::cout << "Grafo vazio." << std::endl;
        return;
    }

    // Printa os  índices da coluna
    std::cout << "\t";
    for (std::size_t column = 0;
        column < adjacencyMatrix_.size();
        column++)
    {
        std::cout << column << "\t";
    }

    std::cout << std::endl;

    for (std::size_t row = 0;
        row < adjacencyMatrix_.size();
        row++)

    {
        std::cout << row << "\t";

        for (float value : adjacencyMatrix_[row])
        {
            std::cout << value << "\t";
        }

        std::cout << std::endl;
    }
}

int AdjacencyMatrixGraph::getVertexCount() const
{
    return static_cast<int>(adjacencyMatrix_.size());
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

    // Peso não pode ser 0
    if (weighted_ && weight == 0.0f)
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

bool AdjacencyMatrixGraph::hasEdge(int source, int destination) const
{
    if (source < 0 ||
        destination < 0 ||
        source >= static_cast<int>(adjacencyMatrix_.size()) ||
        destination >= static_cast<int>(adjacencyMatrix_.size()))
    {
        return false;
    }

	return adjacencyMatrix_[source][destination] != 0.0f;
}

float AdjacencyMatrixGraph::getEdgeWeight(int source, int destination) const
{
    if (source < 0 ||
        destination < 0 ||
        source >= static_cast<int>(adjacencyMatrix_.size()) ||
        destination >= static_cast<int>(adjacencyMatrix_.size()))
    {
		return 0.0f; // Índices forem inválidos
    }

    return adjacencyMatrix_[source][destination];
}

std::vector<int> AdjacencyMatrixGraph::getNeighbors(int vertex) const
{
    std::vector<int> neighbors;

    // Verifica se o vértice é válido
    if (vertex < 0 ||
        vertex >= static_cast<int>(adjacencyMatrix_.size()))
    {
        return neighbors; // Vai retornar vazio
    }

    // Percorre as colunas da linha do vértice
    for (std::size_t destination = 0;
        destination < adjacencyMatrix_[vertex].size();
        ++destination)

    {
        // Verifica se existe aresta nessa posição
        if (adjacencyMatrix_[vertex][destination] != 0.0f)
        {
            neighbors.push_back(
                static_cast<int>(destination)
            );
        }
    }
    return neighbors;
}