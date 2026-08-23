#include "algorithms/BreadthFirstSearch.hpp"

#include <queue>
#include <vector>

std::vector<int> BreadthFirstSearch::execute(
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
	std::queue<int> queue;

	visited[source] = true;
	queue.push(source);

	while (!queue.empty())
	{
		int currentVertex = queue.front();
		queue.pop();

		visitOrder.push_back(currentVertex);

		for (int neighbor : graph.getNeighbors(currentVertex))
		{
			if (!visited[neighbor])
			{
				visited[neighbor] = true;
				queue.push(neighbor);
			}
		}
	}
	return visitOrder;
}