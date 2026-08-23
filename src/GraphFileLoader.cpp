#include "io/GraphFileLoader.hpp"

#include <fstream>
#include <memory>
#include <stdexcept>

#include "graph/AdjacencyMatrixGraph.hpp"
#include "graph/AdjacencyListGraph.hpp"

std::unique_ptr<Graph> GraphFileLoader::loadFromFile(
	const std::string& filePath,
	Representation representation
)
{
	std::ifstream file(filePath);

	if (!file.is_open()) 
	{
		throw std::runtime_error(
			"Nao foi possivel abrir o arquivo: " + filePath
		);
	}

	int vertexCount;
	int edgeCount;
	int directedValue;
	int weightedValue;

	// Lê o header 
	if (!(file >> vertexCount
		>> edgeCount
		>> directedValue
		>> weightedValue))
	{
		throw std::runtime_error(
			"Cabeçalho do arquivo invalido."
		);
	}

	if (vertexCount < 0 || edgeCount < 0)
	{
		throw std::runtime_error(
			"Quantidade de vertices ou arestas invalida."
		);
	}

	if ((directedValue != 0 && directedValue != 1) ||
		(weightedValue != 0 && weightedValue != 1))
	{
		throw std::runtime_error(
			"Os valores de D e P devem ser 0 ou 1."
		);
	}

	const bool directed = directedValue == 1;
	const bool weighted = weightedValue == 1;

	std::unique_ptr<Graph> graph;

	if (representation == Representation::AdjacencyMatrix) 
	{
		graph = std::make_unique<AdjacencyMatrixGraph>
		(
			directed,
			weighted
		);
	}
	else
	{
		graph = std::make_unique<AdjacencyListGraph>(
			directed,
			weighted
		);
	}

	// Cria os V vertices
	for (int i = 0; i < vertexCount; i++)
	{
		if (!graph->addVertex())
		{
			throw std::runtime_error(
				"Erro ao adicionar vertice."
			);
		}
	}
	// Le A arestas
	for (int i = 0; i < edgeCount; i++)
	{
		int source;
		int destination;
		float weight = 1.0f;

		if (weighted)
		{
			// Ao Ad Ap
			if (!(file >> source >> destination >> weight))
			{
				throw std::runtime_error(
					"Aresta ponderada invalida no arquivo."
				);
			}
		}
		else
		{
			// Ao Ad
			if (!(file >> source >> destination))
			{
				throw std::runtime_error(
					"Aresta invalida no arquivo."
				);
			}
		}

		if (!graph->addEdge(source, destination, weight))
		{
			throw std::runtime_error(
				"Nao foi possivel adicionar uma aresta do arquivo."
			);
		}
	}

	return graph;
}