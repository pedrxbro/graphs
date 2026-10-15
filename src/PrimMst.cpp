#include "algorithms/mst/PrimMst.hpp"

#include <limits>
#include <set>
#include <utility>
#include <vector>

MstResult PrimMst::execute(const Graph& graph)
{
    const int vertexCount = graph.getVertexCount();
    const float infinity =
        std::numeric_limits<float>::infinity();

    // key[v]    = cheapest edge weight connecting v to the tree
    // parent[v] = other end of that edge
    std::vector<float> key(vertexCount, infinity);
    std::vector<int> parent(vertexCount, -1);
    std::vector<bool> inTree(vertexCount, false);

    MstResult result;
    int components = 0;

    // Restart from every unvisited vertex so a disconnected
    // graph produces a spanning forest instead of a partial tree.
    for (int start = 0; start < vertexCount; ++start)
    {
        if (inTree[start])
        {
            continue;
        }

        ++components;

        // std::set works as a priority queue: (key, vertex)
        std::set<std::pair<float, int>> queue;

        key[start] = 0.0f;
        queue.insert({ 0.0f, start });

        while (!queue.empty())
        {
            // Extract-min
            const int u = queue.begin()->second;
            queue.erase(queue.begin());

            inTree[u] = true;

            // The root has no parent edge
            if (parent[u] != -1)
            {
                result.edges.push_back(
                    { parent[u], u, key[u] }
                );

                result.totalWeight += key[u];
            }

            for (int v : graph.getNeighbors(u))
            {
                const float weight =
                    graph.getEdgeWeight(u, v);

                if (!inTree[v] && weight < key[v])
                {
                    // Decrease-key = erase old entry + insert new one
                    queue.erase({ key[v], v });

                    key[v] = weight;
                    parent[v] = u;

                    queue.insert({ weight, v });
                }
            }
        }
    }

    result.connected = components <= 1;

    return result;
}
