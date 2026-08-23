#include "graph/AdjacencyListGraph.hpp"

bool AdjacencyListGraph::addVertex()
{
	vertexList_.emplace_back();

	return true;
}

bool AdjacencyListGraph::removeVertex(int index)
{
	// Verifica se é válido
	if (index < 0 || index >= static_cast<int>(vertexList_.size()))
	{
		return false;
	}

	// Remove o vértice e todas as arestas que saem dele
	vertexList_.erase(vertexList_.begin() + index);

	// Atualiza as conexões dos vértices que sobraram
	for (Vertex& vertex : vertexList_)
	{
		std::vector<Edge>& connections = vertex.connections;

		for (
			std::vector<Edge>::iterator it = connections.begin();
			it != connections.end();
			)
		{
			// Remove arestas que chegavam ao vértice que foi removido
			if (it->destination == index)
			{
				it = connections.erase(it);
			}
			else
			{
				// Ajusta os índices após remoção
				if (it->destination > index)
				{
					--it->destination;
				}

				++it;
			}
		}
	}

	return true;
}

bool AdjacencyListGraph::addEdge(int source, int destination, float weight)
{
	// Verifica se os índices são válidos
	if( source < 0 ||
		destination < 0 ||
		source >= static_cast<int>(vertexList_.size()) ||
		destination >= static_cast<int>(vertexList_.size()))
	{ 
		return false;
	}

	// Peso não pode ser 0
	if (weighted_ && weight == 0.0f)
	{
		return false;
	}
	const float edgeWeight = weighted_ ? weight : 1.0f;

	std::vector<Edge>& sourceConnections = vertexList_[source].connections;

	// Verifica se a aresta já existe
	for (Edge& edge : sourceConnections)
	{
		if (edge.destination == destination)
		{
			// Se existe atualiza somente o peso
			edge.weight = edgeWeight;

			// Atualiza a ligação de volta
			if (!directed_ && source != destination)
			{
				std::vector<Edge>& destinationConnections =
					vertexList_[destination].connections;

				// Procura as arestas que saem do destino e atualiza o peso
				for (Edge& reverseEdge : destinationConnections)
				{
					if (reverseEdge.destination == source)
					{
						reverseEdge.weight = edgeWeight;
						break;
					}
				}
			}
			return true;
		}
	}

	// Adiciona a aresta
	sourceConnections.push_back({destination,edgeWeight});

	// Adiciona a volta para não direcionados
	if (!directed_ && source != destination)
	{
		vertexList_[destination].connections.push_back({source, edgeWeight});
	}

	return true;
}

bool AdjacencyListGraph::removeEdge(int source, int destination)
{
	if (source < 0 ||
		destination < 0 ||
		source >= static_cast<int>(vertexList_.size()) ||
		destination >= static_cast<int>(vertexList_.size()))
	{
		return false;
	}

	std::vector<Edge>& sourceConnections =
		vertexList_[source].connections;

	bool removed = false;

	// Procura e remove aresta de origem para destino
	for (std::vector<Edge>::iterator it = sourceConnections.begin();
		it != sourceConnections.end();
		++it
		)
	{
		if (it->destination == destination)
		{
			sourceConnections.erase(it);
			removed = true;
			break;
		}
	}
	if (!removed) {
		removed = false;
	}

	// Remove a volta
	if (!directed_ && source != destination) 
	{
		std::vector<Edge>& destinationConnections =
			vertexList_[destination].connections;

		for (std::vector<Edge>::iterator it = destinationConnections.begin();
			it != destinationConnections.end();
			++it
			)
		{
			if (it->destination == source) {
				destinationConnections.erase(it);
				break;
			}
		}
	}
	return true;
}

bool AdjacencyListGraph::hasEdge(int source, int destination) const
{
	if (source < 0 ||
		destination < 0 ||
		source >= static_cast<int>(vertexList_.size()) ||
		destination >= static_cast<int>(vertexList_.size()))
	{
		return false;
	}

	const std::vector<Edge>& connections =
		vertexList_[source].connections;

	// Procura a aresta
	for (const Edge& edge : connections) 
	{
		if (edge.destination == destination)
		{
			return true;
		}
	}
	return false;
}

float AdjacencyListGraph::getEdgeWeight(int source, int destination) const
{
	if (source < 0 ||
		destination < 0 || 
		source >= static_cast<int>(vertexList_.size()) ||
		destination >= static_cast<int>(vertexList_.size()))
	{
		return 0.0f;
	}

	const std::vector<Edge> connections =
		vertexList_[source].connections;

	// Procura a aresta
	for (const Edge& edge : connections)
	{
		if (edge.destination == destination) 
		{
			return edge.weight;
		}
	}

	return 0.0f;
}