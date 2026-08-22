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