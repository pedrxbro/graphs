#include "algorithms/flow/LocalSearchFlow.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

#include "algorithms/flow/FordFulkerson.hpp"
#include "graph/AdjacencyListGraph.hpp"

namespace
{
    // Representa uma aresta individual da solucao.
    // Sua identidade e preservada durante a busca.
    struct FlowEdge
    {
        int source;
        int destination;
        float capacity;
    };

    // Extrai as arestas sem modificar o grafo original.
    std::vector<FlowEdge> collectEdges(
        const Graph& graph
    )
    {
        std::vector<FlowEdge> edges;

        const int vertexCount = graph.getVertexCount();

        for (int source = 0;
            source < vertexCount;
            ++source)
        {
            for (int destination :
            graph.getNeighbors(source))
            {
                if (destination < 0 ||
                    destination >= vertexCount)
                {
                    throw std::invalid_argument(
                        "Grafo possui vizinho invalido."
                    );
                }

                const float capacity =
                    graph.getEdgeWeight(
                        source,
                        destination
                    );

                if (!std::isfinite(capacity) ||
                    capacity < 0.0f)
                {
                    throw std::invalid_argument(
                        "Capacidades devem ser finitas "
                        "e nao negativas."
                    );
                }

                edges.push_back({
                    source,
                    destination,
                    capacity
                    });
            }
        }

        // Ordem canonica para desempate deterministico,
        // independentemente da representacao do grafo.
        std::sort(
            edges.begin(),
            edges.end(),
            [](const FlowEdge& a, const FlowEdge& b)
            {
                if (a.source != b.source)
                {
                    return a.source < b.source;
                }

                return a.destination < b.destination;
            }
        );

        return edges;
    }

    // Constroi um grafo independente e avalia seu fluxo.
    double evaluateSolution(
        const std::vector<FlowEdge>& edges,
        int vertexCount,
        int source,
        int sink
    )
    {
        AdjacencyListGraph candidate(true, true);

        // Preserva todos os vertices, inclusive isolados.
        for (int vertex = 0;
            vertex < vertexCount;
            ++vertex)
        {
            if (!candidate.addVertex())
            {
                throw std::runtime_error(
                    "Falha ao criar vertice candidato."
                );
            }
        }

        // O Graph armazena apenas uma aresta por
        // par ordenado. Por isso, capacidades de
        // arcos coincidentes devem ser agregadas.
        std::map<std::pair<int, int>, double>
            aggregatedCapacities;

        for (const FlowEdge& edge : edges)
        {
            // Capacidade zero nao transporta fluxo.
            if (edge.capacity == 0.0f)
            {
                continue;
            }

            const std::pair<int, int> endpoints =
                std::make_pair(
                    edge.source,
                    edge.destination
                );

            double& total =
                aggregatedCapacities[endpoints];

            total += static_cast<double>(
                edge.capacity
                );

            if (!std::isfinite(total) ||
                total >
                static_cast<double>(
                    std::numeric_limits<float>::max()
                    ))
            {
                throw std::overflow_error(
                    "Capacidade agregada excede "
                    "o limite de float."
                );
            }
        }

        // Insere cada par ordenado uma unica vez.
        // A soma e realizada antes de addEdge,
        // que sobrescreveria uma aresta existente.
        for (const auto& entry :
            aggregatedCapacities)
        {
            const int from = entry.first.first;
            const int to = entry.first.second;

            const float capacity =
                static_cast<float>(entry.second);

            if (capacity <= 0.0f)
            {
                throw std::overflow_error(
                    "Capacidade candidata invalida."
                );
            }

            if (!candidate.addEdge(
                from,
                to,
                capacity
            ))
            {
                throw std::runtime_error(
                    "Falha ao adicionar aresta candidata."
                );
            }
        }

        // Reutiliza o algoritmo da Etapa 01.
        return FordFulkerson::execute(
            candidate,
            source,
            sink
        );
    }
}

LocalSearchResult LocalSearchFlow::execute(
    const Graph& graph,
    int source,
    int sink
)
{
    // Avalia a estrutura original.
    // Ford-Fulkerson tambem valida os argumentos.
    const double initialFlow =
        FordFulkerson::execute(
            graph,
            source,
            sink
        );

    const int vertexCount =
        graph.getVertexCount();

    // Mantem a identidade individual das arestas.
    std::vector<FlowEdge> currentEdges =
        collectEdges(graph);

    double currentFlow = initialFlow;
    int steps = 0;

    // Busca local por melhor melhora.
    while (true)
    {
        double bestFlow = currentFlow;

        // Indice invalido significa que nenhuma
        // melhoria foi encontrada nesta iteracao.
        std::size_t bestEdgeIndex =
            currentEdges.size();

        // Avalia todos os vizinhos da solucao atual.
        for (std::size_t i = 0;
            i < currentEdges.size();
            ++i)
        {
            // Inverter um self-loop nao altera
            // a estrutura do grafo.
            if (currentEdges[i].source ==
                currentEdges[i].destination)
            {
                continue;
            }

            // Cada vizinho parte da solucao corrente,
            // nunca de outro vizinho em avaliacao.
            std::vector<FlowEdge> neighborEdges =
                currentEdges;

            // Exatamente uma aresta e invertida.
            // Sua capacidade permanece inalterada.
            std::swap(
                neighborEdges[i].source,
                neighborEdges[i].destination
            );

            // Constroi um candidato independente
            // e calcula o fluxo maximo.
            const double neighborFlow =
                evaluateSolution(
                    neighborEdges,
                    vertexCount,
                    source,
                    sink
                );

            // Apenas melhoria estrita e considerada.
            // Em empate, permanece a primeira aresta
            // encontrada na ordem canonica.
            if (neighborFlow > bestFlow)
            {
                bestFlow = neighborFlow;
                bestEdgeIndex = i;
            }
        }

        // Nenhum vizinho melhor: otimo local.
        if (bestEdgeIndex == currentEdges.size())
        {
            break;
        }

        if (steps == std::numeric_limits<int>::max())
        {
            throw std::overflow_error(
                "Numero de passos excede "
                "o limite de int."
            );
        }

        // Aplica somente a melhor inversao aceita.
        // As demais arestas nao sao modificadas.
        std::swap(
            currentEdges[bestEdgeIndex].source,
            currentEdges[bestEdgeIndex].destination
        );

        currentFlow = bestFlow;

        // Um passo corresponde a uma inversao aceita.
        ++steps;
    }

    return {
        initialFlow,
        currentFlow,
        steps
    };
}