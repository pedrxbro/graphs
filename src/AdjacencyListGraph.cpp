#include "graph/AdjacencyListGraph.hpp"

bool AdjacencyListGraph::addVertex() 
{
	vertexList_.emplace_back();

	return true;
}