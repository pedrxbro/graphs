
#include "algorithms/flow/FordFulkerson.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

#include "algorithms/DepthFirstSearch.hpp"
#include "graph/AdjacencyListGraph.hpp"

double FordFulkerson::execute(
    const Graph& graph,
    int source,
    int sink
)
{
    const int vertexCount = graph.getVertexCount();

    if (source < 0 ||
        sink < 0 ||
        source >= vertexCount ||
        sink >= vertexCount)
    {
        throw std::invalid_argument(
            "Fonte ou destino invalido."
        );
    }

    if (source == sink)
    {
        throw std::invalid_argument(
            "Fonte e destino devem ser distintos."
        );
    }

    if (!graph.isDirected() || !graph.isWeighted())
    {
        throw std::invalid_argument(
            "Ford-Fulkerson exige um grafo "
            "direcionado e ponderado."
        );
    }

    // Rede residual independente do grafo original.
    AdjacencyListGraph residual(true, true);

    for (int vertex = 0; vertex < vertexCount; ++vertex)
    {
        if (!residual.addVertex())
        {
            throw std::runtime_error(
                "Falha ao criar vertice residual."
            );
        }
    }

    // Copia somente arcos de capacidade positiva.
    // Antes, valida todas as capacidades originais.
    for (int vertex = 0; vertex < vertexCount; ++vertex)
    {
        for (int neighbor : graph.getNeighbors(vertex))
        {
            if (neighbor < 0 || neighbor >= vertexCount)
            {
                throw std::invalid_argument(
                    "Grafo possui vizinho invalido."
                );
            }

            const float capacity =
                graph.getEdgeWeight(vertex, neighbor);

            if (!std::isfinite(capacity) ||
                capacity < 0.0f)
            {
                throw std::invalid_argument(
                    "Capacidades devem ser finitas "
                    "e nao negativas."
                );
            }

            if (capacity == 0.0f)
            {
                continue;
            }

            if (!residual.addEdge(
                vertex,
                neighbor,
                capacity
            ))
            {
                throw std::runtime_error(
                    "Falha ao construir rede residual."
                );
            }
        }
    }

    double maxFlow = 0.0;

    while (true)
    {
        const std::vector<int> path =
            DepthFirstSearch::findPath(
                residual,
                source,
                sink
            );

        // Nao existem mais caminhos aumentantes.
        if (path.empty())
        {
            break;
        }

        // Menor capacidade residual do caminho.
        double bottleneck =
            std::numeric_limits<double>::infinity();

        for (std::size_t i = 1; i < path.size(); ++i)
        {
            const int from = path[i - 1];
            const int to = path[i];

            const float capacity =
                residual.getEdgeWeight(from, to);

            if (!std::isfinite(capacity) ||
                capacity <= 0.0f)
            {
                throw std::logic_error(
                    "Caminho aumentante invalido."
                );
            }

            bottleneck = std::min(
                bottleneck,
                static_cast<double>(capacity)
            );
        }

        if (!std::isfinite(bottleneck) ||
            bottleneck <= 0.0)
        {
            throw std::logic_error(
                "Capacidade aumentante invalida."
            );
        }

        // Atualiza cada arco direto e seu reverso.
        for (std::size_t i = 1; i < path.size(); ++i)
        {
            const int from = path[i - 1];
            const int to = path[i];

            const float forwardCapacity =
                residual.getEdgeWeight(from, to);

            // Pode existir um arco reverso original.
            // Sua capacidade precisa ser acumulada.
            const float reverseCapacity =
                residual.getEdgeWeight(to, from);

            const double remainingCapacity =
                static_cast<double>(forwardCapacity) -
                bottleneck;

            const double updatedReverseCapacity =
                static_cast<double>(reverseCapacity) +
                bottleneck;

            // A representacao de grafo usa float.
            if (!std::isfinite(updatedReverseCapacity) ||
                updatedReverseCapacity >
                std::numeric_limits<float>::max())
            {
                throw std::overflow_error(
                    "Capacidade residual reversa "
                    "excede o limite de float."
                );
            }

            const float forwardUpdated =
                static_cast<float>(remainingCapacity);

            const float reverseUpdated =
                static_cast<float>(
                    updatedReverseCapacity
                    );

            if (forwardUpdated == forwardCapacity ||
                reverseUpdated <= reverseCapacity)
            {
                throw std::overflow_error(
                    "Precisao insuficiente para "
                    "atualizar a rede residual."
                );
            }

            // Remove o arco quando esgotado.
            if (forwardUpdated <= 0.0f)
            {
                if (!residual.removeEdge(from, to))
                {
                    throw std::logic_error(
                        "Falha ao remover arco esgotado."
                    );
                }
            }
            else
            {
                if (!residual.addEdge(
                    from,
                    to,
                    forwardUpdated
                ))
                {
                    throw std::runtime_error(
                        "Falha ao atualizar arco direto."
                    );
                }
            }

            // addEdge sobrescreve pesos existentes,
            // portanto a soma foi calculada acima.
            if (!residual.addEdge(
                to,
                from,
                reverseUpdated
            ))
            {
                throw std::runtime_error(
                    "Falha ao atualizar arco reverso."
                );
            }
        }

        const double updatedMaxFlow =
            maxFlow + bottleneck;

        if (!std::isfinite(updatedMaxFlow))
        {
            throw std::overflow_error(
                "Fluxo total excedeu o limite numerico."
            );
        }

        maxFlow = updatedMaxFlow;
    }

    return maxFlow;
}
