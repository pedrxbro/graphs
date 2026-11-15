#include <cmath>
#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "algorithms/DepthFirstSearch.hpp"
#include "algorithms/flow/FordFulkerson.hpp"
#include "graph/AdjacencyListGraph.hpp"
#include "graph/AdjacencyMatrixGraph.hpp"
#include "io/GraphFileLoader.hpp"

namespace
{
    void require(
        bool condition,
        const std::string& message
    )
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }

    template <typename ExpectedException, typename Action>
    void expectException(
        Action action,
        const std::string& message
    )
    {
        bool exceptionReceived = false;

        try
        {
            action();
        }
        catch (const ExpectedException&)
        {
            exceptionReceived = true;
        }

        require(exceptionReceived, message);
    }

    void addVertices(Graph& graph, int count)
    {
        for (int i = 0; i < count; ++i)
        {
            require(
                graph.addVertex(),
                "Falha ao criar vertice do teste."
            );
        }
    }

    struct GraphSnapshot
    {
        std::vector<std::vector<int>> neighbors;
        std::vector<std::vector<float>> weights;
    };

    GraphSnapshot captureGraph(const Graph& graph)
    {
        GraphSnapshot snapshot;

        const int count = graph.getVertexCount();

        for (int vertex = 0; vertex < count; ++vertex)
        {
            snapshot.neighbors.push_back(
                graph.getNeighbors(vertex)
            );

            std::vector<float> row;

            for (int destination = 0;
                destination < count;
                ++destination)
            {
                row.push_back(
                    graph.getEdgeWeight(
                        vertex,
                        destination
                    )
                );
            }

            snapshot.weights.push_back(row);
        }

        return snapshot;
    }

    void assertGraphUnchanged(
        const Graph& graph,
        const GraphSnapshot& before
    )
    {
        const GraphSnapshot after = captureGraph(graph);

        require(
            before.neighbors == after.neighbors,
            "A lista de vizinhos original foi alterada."
        );

        require(
            before.weights == after.weights,
            "Os pesos originais foram alterados."
        );
    }

    void testClassicGraph(
        const std::string& filePath,
        GraphFileLoader::Representation representation
    )
    {
        std::unique_ptr<Graph> graph =
            GraphFileLoader::loadFromFile(
                filePath,
                representation
            );

        require(
            graph->getVertexCount() == 6,
            "O grafo classico deve ter seis vertices."
        );

        require(
            graph->isDirected() && graph->isWeighted(),
            "O grafo deve ser direcionado e ponderado."
        );

        // A capacidade zero declarada no arquivo
        // deve ser tratada como arco inativo.
        require(
            !graph->hasEdge(0, 5),
            "O loader nao ignorou a capacidade zero."
        );

        const GraphSnapshot before = captureGraph(*graph);

        // Garante que execute mantenha a ordem da M1.
        const std::vector<int> expectedOrder = {
            0, 1, 2, 4, 3, 5
        };

        require(
            DepthFirstSearch::execute(*graph, 0) ==
            expectedOrder,
            "A ordem original da DFS foi alterada."
        );

        // A DFS deve encontrar um caminho valido.
        const std::vector<int> path =
            DepthFirstSearch::findPath(
                *graph,
                0,
                5
            );

        require(
            !path.empty(),
            "A DFS nao encontrou caminho aumentante."
        );

        require(
            path.front() == 0 && path.back() == 5,
            "O caminho nao inclui fonte e destino."
        );

        std::vector<bool> visited(6, false);

        for (std::size_t i = 0; i < path.size(); ++i)
        {
            const int vertex = path[i];

            require(
                vertex >= 0 && vertex < 6,
                "Caminho possui vertice invalido."
            );

            require(
                !visited[vertex],
                "Caminho possui vertice repetido."
            );

            visited[vertex] = true;

            if (i > 0)
            {
                require(
                    graph->getEdgeWeight(
                        path[i - 1],
                        vertex
                    ) > 0.0f,
                    "Caminho possui arco sem capacidade."
                );
            }
        }

        const double firstResult =
            FordFulkerson::execute(*graph, 0, 5);

        require(
            firstResult == 23.0,
            "Fluxo classico diferente de 23."
        );

        assertGraphUnchanged(*graph, before);

        // Repetir deve produzir o mesmo resultado.
        const double secondResult =
            FordFulkerson::execute(*graph, 0, 5);

        require(
            secondResult == 23.0,
            "Execucao repetida produziu fluxo diferente."
        );

        assertGraphUnchanged(*graph, before);

        std::cout
            << "[OK] Exemplo classico: fluxo 23\n";
    }

    void testDfsBacktracking()
    {
        AdjacencyListGraph graph(true, true);
        addVertices(graph, 4);

        graph.addEdge(0, 1, 1.0f);
        graph.addEdge(0, 2, 2.0f);
        graph.addEdge(2, 3, 2.0f);

        const std::vector<int> expectedPath = {
            0, 2, 3
        };

        require(
            DepthFirstSearch::findPath(
                graph,
                0,
                3
            ) == expectedPath,
            "A DFS nao removeu o ramo sem saida."
        );

        const std::vector<int> expectedOrder = {
            0, 1, 2, 3
        };

        require(
            DepthFirstSearch::execute(
                graph,
                0
            ) == expectedOrder,
            "A DFS da M1 perdeu a ordem de visita."
        );

        require(
            DepthFirstSearch::findPath(
                graph,
                3,
                0
            ).empty(),
            "A DFS ignorou a direcao dos arcos."
        );

        require(
            DepthFirstSearch::findPath(
                graph,
                0,
                0
            ) == std::vector<int>{0},
            "Caminho de um vertice para si incorreto."
        );

        std::cout
            << "[OK] DFS e retrocesso\n";
    }

    void testAntiparallelEdges()
    {
        AdjacencyListGraph graph(true, true);
        addVertices(graph, 4);

        graph.addEdge(0, 1, 3.0f);
        graph.addEdge(0, 2, 2.0f);
        graph.addEdge(1, 2, 1.0f);
        graph.addEdge(2, 1, 1.0f);
        graph.addEdge(1, 3, 2.0f);
        graph.addEdge(2, 3, 3.0f);

        const GraphSnapshot before = captureGraph(graph);

        const double result =
            FordFulkerson::execute(graph, 0, 3);

        require(
            result == 5.0,
            "Fluxo incorreto com arcos antiparalelos."
        );

        assertGraphUnchanged(graph, before);

        std::cout
            << "[OK] Arcos reversos preexistentes\n";
    }

    void testDirection()
    {
        AdjacencyListGraph graph(true, true);
        addVertices(graph, 3);

        graph.addEdge(1, 0, 3.0f);
        graph.addEdge(1, 2, 3.0f);

        require(
            DepthFirstSearch::findPath(
                graph,
                0,
                2
            ).empty(),
            "A DFS percorreu arco no sentido contrario."
        );

        require(
            FordFulkerson::execute(
                graph,
                0,
                2
            ) == 0.0,
            "Fluxo incorreto em grafo sem caminho."
        );

        std::cout
            << "[OK] Direcao dos arcos\n";
    }

    void testFractionalCapacities()
    {
        AdjacencyListGraph graph(true, true);
        addVertices(graph, 3);

        graph.addEdge(0, 1, 0.5f);
        graph.addEdge(1, 2, 0.25f);
        graph.addEdge(0, 2, 0.125f);

        const double result =
            FordFulkerson::execute(graph, 0, 2);

        require(
            std::fabs(result - 0.375) < 1e-6,
            "Fluxo fracionario incorreto."
        );

        std::cout
            << "[OK] Capacidades fracionarias\n";
    }

    void testInvalidArguments()
    {
        AdjacencyListGraph graph(true, true);
        addVertices(graph, 2);
        graph.addEdge(0, 1, 3.0f);

        expectException<std::invalid_argument>(
            [&]()
            {
                FordFulkerson::execute(graph, -1, 1);
            },
            "Fonte negativa deveria ser rejeitada."
        );

        expectException<std::invalid_argument>(
            [&]()
            {
                FordFulkerson::execute(graph, 0, 2);
            },
            "Destino fora do intervalo deveria falhar."
        );

        expectException<std::invalid_argument>(
            [&]()
            {
                FordFulkerson::execute(graph, 0, 0);
            },
            "Fonte igual ao destino deveria falhar."
        );

        AdjacencyListGraph undirected(false, true);
        addVertices(undirected, 2);
        undirected.addEdge(0, 1, 3.0f);

        expectException<std::invalid_argument>(
            [&]()
            {
                FordFulkerson::execute(undirected, 0, 1);
            },
            "Grafo nao direcionado deveria falhar."
        );

        AdjacencyListGraph unweighted(true, false);
        addVertices(unweighted, 2);
        unweighted.addEdge(0, 1);

        expectException<std::invalid_argument>(
            [&]()
            {
                FordFulkerson::execute(unweighted, 0, 1);
            },
            "Grafo nao ponderado deveria falhar."
        );

        std::cout
            << "[OK] Validacao de parametros\n";
    }

    void testInvalidCapacities()
    {
        AdjacencyListGraph negative(true, true);
        addVertices(negative, 3);

        negative.addEdge(0, 1, -2.0f);
        negative.addEdge(1, 2, 1.0f);

        // execute da M1 continua ignorando pesos.
        require(
            DepthFirstSearch::execute(
                negative,
                0
            ) == std::vector<int>({ 0, 1, 2 }),
            "A DFS original deveria visitar todos."
        );

        // findPath nao pode atravessar capacidade negativa.
        require(
            DepthFirstSearch::findPath(
                negative,
                0,
                2
            ).empty(),
            "findPath percorreu capacidade negativa."
        );

        expectException<std::invalid_argument>(
            [&]()
            {
                FordFulkerson::execute(
                    negative,
                    0,
                    2
                );
            },
            "Capacidade negativa deveria falhar."
        );

        const std::vector<float> invalidValues = {
            std::numeric_limits<float>::infinity(),
            std::numeric_limits<float>::quiet_NaN()
        };

        for (float capacity : invalidValues)
        {
            AdjacencyListGraph invalid(true, true);
            addVertices(invalid, 2);

            require(
                invalid.addEdge(0, 1, capacity),
                "Falha ao preparar capacidade invalida."
            );

            expectException<std::invalid_argument>(
                [&]()
                {
                    FordFulkerson::execute(
                        invalid,
                        0,
                        1
                    );
                },
                "Capacidade nao finita deveria falhar."
            );
        }

        std::cout
            << "[OK] Capacidades invalidas\n";
    }

    void testDisconnectedGraph()
    {
        AdjacencyMatrixGraph graph(true, true);
        addVertices(graph, 3);

        graph.addEdge(0, 1, 5.0f);

        require(
            FordFulkerson::execute(graph, 0, 2) == 0.0,
            "Grafo sem caminho deveria ter fluxo zero."
        );

        std::cout
            << "[OK] Grafo desconexo\n";
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr
            << "Uso: graphs_flow_tests "
            << "<arquivo_do_grafo>\n";

        return 1;
    }

    try
    {
        const std::string filePath = argv[1];

        testClassicGraph(
            filePath,
            GraphFileLoader::Representation::AdjacencyList
        );

        testClassicGraph(
            filePath,
            GraphFileLoader::Representation::AdjacencyMatrix
        );

        testDfsBacktracking();
        testAntiparallelEdges();
        testDirection();
        testFractionalCapacities();
        testInvalidArguments();
        testInvalidCapacities();
        testDisconnectedGraph();

        std::cout
            << "\nTodos os testes da M3.1 passaram.\n";

        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cerr
            << "[FALHOU] "
            << exception.what()
            << '\n';

        return 1;
    }
}