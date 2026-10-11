#include "algorithms/mst/MstValidator.hpp"

bool MstValidator::isUndirected(const Graph& graph)
{
    const int vertexCount = graph.getVertexCount();

    for (int u = 0; u < vertexCount; ++u)
    {
        for (int v : graph.getNeighbors(u))
        {
            if (!graph.hasEdge(v, u))
            {
                return false;
            }
        }
    }

    return true;
}
