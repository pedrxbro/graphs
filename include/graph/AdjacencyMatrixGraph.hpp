#pragma once

#include <vector>

#include "graph/Graph.hpp"

class AdjacencyMatrixGraph : public Graph
{
public: 
	AdjacencyMatrixGraph(bool directed, bool weighted)
		: Graph(directed, weighted)

	{
	}

	bool addVertex() override;

private:
	std::vector<std::vector<float>> adjacencyMatrix_;
};