#include "algorithms/mst/KruskalMst.hpp"

#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

namespace
{
    // Disjoint set with path compression and union by rank.
    // Without it the cycle check would cost O(V) per edge.
    class DisjointSet
    {
    public:
        explicit DisjointSet(int size)
            : parent_(size),
            rank_(size, 0)
        {
            std::iota(parent_.begin(), parent_.end(), 0);
        }

        int find(int x)
        {
            if (parent_[x] != x)
            {
                parent_[x] = find(parent_[x]);
            }

            return parent_[x];
        }

        // false = already in the same set (edge would close a cycle)
        bool unite(int a, int b)
        {
            a = find(a);
            b = find(b);

            if (a == b)
            {
                return false;
            }

            if (rank_[a] < rank_[b])
            {
                std::swap(a, b);
            }

            parent_[b] = a;

            if (rank_[a] == rank_[b])
            {
                ++rank_[a];
            }

            return true;
        }

    private:
        std::vector<int> parent_;
        std::vector<int> rank_;
    };
}

MstResult KruskalMst::execute(const Graph& graph)
{
    const int vertexCount = graph.getVertexCount();

    // Collect each undirected edge once (u < v).
    // Self loops are skipped, they never belong to an MST.
    std::vector<MstEdge> edges;

    for (int u = 0; u < vertexCount; ++u)
    {
        for (int v : graph.getNeighbors(u))
        {
            if (u < v)
            {
                edges.push_back(
                    { u, v, graph.getEdgeWeight(u, v) }
                );
            }
        }
    }

    std::sort(
        edges.begin(),
        edges.end(),
        [](const MstEdge& a, const MstEdge& b)
        {
            return a.weight < b.weight;
        }
    );

    MstResult result;
    DisjointSet sets(vertexCount);

    for (const MstEdge& edge : edges)
    {
        if (sets.unite(edge.source, edge.destination))
        {
            result.edges.push_back(edge);
            result.totalWeight += edge.weight;

            // A spanning tree has exactly V - 1 edges
            if (static_cast<int>(result.edges.size()) ==
                vertexCount - 1)
            {
                break;
            }
        }
    }

    result.connected =
        vertexCount <= 1 ||
        static_cast<int>(result.edges.size()) ==
        vertexCount - 1;

    return result;
}
