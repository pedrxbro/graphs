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
	bool removeVertex(int index) override;
	
	bool addEdge(
		int source, 
		int destination,	
		float weight = 1.0f
	) override;

	bool removeEdge(
		int source,
		int destination
	) override;


	bool hasEdge(
		int source,
		int destination
	) const override;

	float getEdgeWeight(
		int source,
		int destination
	) const override;

	std::vector<int> getNeighbors(
		int vertex
	) const override;

private:
	std::vector<std::vector<float>> adjacencyMatrix_;
};