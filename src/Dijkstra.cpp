#include "algorithms/Dijkstra.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

DijkstraResult Dijkstra::execute(const Graph& graph, int source)
{
    DijkstraResult result;

    int vertexCount = graph.getVertexCount();

    if (source < 0 || source >= vertexCount)
    {
        return result;
    }

    result.distances.resize(vertexCount, std::numeric_limits<float>::infinity());
    result.previous.resize(vertexCount, -1);
    result.distances[source] = 0.0f;

    std::vector<bool> closed(vertexCount, false);

    for (int step = 0; step < vertexCount; ++step)
    {
        int currentVertex = -1;

        float smallestDistance = std::numeric_limits<float>::infinity();

        // Procura o vertice aberto com menor distancia
        for (int vertex = 0; vertex < vertexCount; ++vertex)
        {
            if (!closed[vertex] && result.distances[vertex] < smallestDistance)
            {
                smallestDistance = result.distances[vertex];

                currentVertex = vertex;
            }
        }

		// Se não houver mais vertices abertos, termina
		if (currentVertex == -1)
		{
			break;
		}

		// Atualiza as distancias dos vizinhos do vertice atual
        for (int neighbor : graph.getNeighbors(currentVertex))
        {
            float weight = graph.getEdgeWeight(currentVertex,neighbor);

            if (weight < 0.0f)
            {
                throw std::invalid_argument("Dijkstra nao suporta arestas com peso negativo.");
            }

            float newDistance = result.distances[currentVertex] + weight;

			// Se a nova distancia for menor, atualiza a distancia e o vertice anterior
            if (newDistance < result.distances[neighbor])
            {
                result.distances[neighbor] = newDistance;

                result.previous[neighbor] = currentVertex;
            }
        }

        closed[currentVertex] = true;
    }

    return result;
}

std::vector<int> Dijkstra::buildPath(
    const DijkstraResult& result,
    int source,
    int destination
)
{
    std::vector<int> path;

    if (source < 0 ||
        destination < 0 ||
        source >= static_cast<int>(
            result.distances.size()
            ) ||
        destination >= static_cast<int>(
            result.distances.size()
            ))
    {
        return path;
    }

    int currentVertex = destination;

    while (currentVertex != -1)
    {
        path.push_back(currentVertex);

        if (currentVertex == source)
        {
            break;
        }

        currentVertex =
            result.previous[currentVertex];
    }

    // Nao chegou ate a origem:
    // destino era inalcançavel
    if (path.empty() ||
        path.back() != source)
    {
        path.clear();
        return path;
    }

    std::reverse(
        path.begin(),
        path.end()
    );

    return path;
}