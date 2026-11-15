#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include "algorithms/flow/LocalSearchFlow.hpp"
#include "graph/AdjacencyListGraph.hpp"
#include "graph/AdjacencyMatrixGraph.hpp"

namespace
{
    constexpr double tolerance = 1e-6;

    struct EdgeSpec
    {
        int source;
        int destination;
        float capacity;
    };

    struct Scenario
    {
        std::string name;
        int vertexCount;
        std::vector<EdgeSpec> edges;
        int source;
        int sink;
        double expectedInitialFlow;
        double expectedFinalFlow;
        int expectedSteps;
    };

    struct GraphSnapshot
    {
        bool directed;
        bool weighted;
        int vertexCount;
        std::vector<std::vector<int>> neighbors;
        std::vector<std::vector<float>> capacities;
    };

    class TestRunner
    {
    public:
        void run(
            const std::string& name,
            const std::string& representation,
            const std::function<void()>& action
        )
        {
            ++total_;

            try
            {
                action();
                ++passed_;
                std::cout << "[OK] " << name << " - "
                          << representation << '\n';
            }
            catch (const std::exception& exception)
            {
                ++failed_;
                std::cerr << "[FALHOU] " << name << " - "
                          << representation << ": "
                          << exception.what() << '\n';
            }
            catch (...)
            {
                ++failed_;
                std::cerr << "[FALHOU] " << name << " - "
                          << representation
                          << ": excecao desconhecida.\n";
            }
        }

        int finish() const
        {
            std::cout << "\n[RESUMO]\n"
                      << "Total de verificacoes: " << total_ << '\n'
                      << "Aprovadas: " << passed_ << '\n'
                      << "Reprovadas: " << failed_ << '\n';

            return failed_ == 0 ? 0 : 1;
        }

    private:
        int total_ = 0;
        int passed_ = 0;
        int failed_ = 0;
    };

    std::string toText(double value)
    {
        std::ostringstream output;
        output.precision(12);
        output << value;
        return output.str();
    }

    void require(bool condition, const std::string& message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }

    void requireEqual(
        int actual,
        int expected,
        const std::string& field
    )
    {
        if (actual != expected)
        {
            throw std::runtime_error(
                field + ": esperado = " +
                std::to_string(expected) +
                ", obtido = " +
                std::to_string(actual) + "."
            );
        }
    }

    void requireClose(
        double actual,
        double expected,
        const std::string& field
    )
    {
        if (!std::isfinite(actual) ||
            std::fabs(actual - expected) > tolerance)
        {
            throw std::runtime_error(
                field + ": esperado = " +
                toText(expected) +
                ", obtido = " +
                toText(actual) + "."
            );
        }
    }

    void requireSameResult(
        const LocalSearchResult& actual,
        const LocalSearchResult& expected,
        const std::string& context
    )
    {
        requireClose(
            actual.initialFlow,
            expected.initialFlow,
            context + " initialFlow"
        );
        requireClose(
            actual.finalFlow,
            expected.finalFlow,
            context + " finalFlow"
        );
        requireEqual(
            actual.steps,
            expected.steps,
            context + " steps"
        );
    }

    void requireExpectedResult(
        const LocalSearchResult& result,
        const Scenario& scenario
    )
    {
        requireClose(
            result.initialFlow,
            scenario.expectedInitialFlow,
            scenario.name + " initialFlow"
        );
        requireClose(
            result.finalFlow,
            scenario.expectedFinalFlow,
            scenario.name + " finalFlow"
        );
        requireEqual(
            result.steps,
            scenario.expectedSteps,
            scenario.name + " steps"
        );
    }

    void addVertices(Graph& graph, int count)
    {
        for (int vertex = 0; vertex < count; ++vertex)
        {
            require(
                graph.addVertex(),
                "Falha ao adicionar o vertice " +
                std::to_string(vertex) + "."
            );
        }
    }

    void addEdges(
        Graph& graph,
        const std::vector<EdgeSpec>& edges
    )
    {
        for (const EdgeSpec& edge : edges)
        {
            require(
                graph.addEdge(
                    edge.source,
                    edge.destination,
                    edge.capacity
                ),
                "Falha ao adicionar a aresta " +
                std::to_string(edge.source) + " -> " +
                std::to_string(edge.destination) + "."
            );
        }
    }

    template <typename GraphType>
    GraphType buildGraph(
        const Scenario& scenario,
        bool directed = true,
        bool weighted = true
    )
    {
        GraphType graph(directed, weighted);
        addVertices(graph, scenario.vertexCount);
        addEdges(graph, scenario.edges);
        return graph;
    }

    GraphSnapshot captureGraph(const Graph& graph)
    {
        GraphSnapshot snapshot{
            graph.isDirected(),
            graph.isWeighted(),
            graph.getVertexCount(),
            {},
            {}
        };

        for (int source = 0;
            source < snapshot.vertexCount;
            ++source)
        {
            snapshot.neighbors.push_back(
                graph.getNeighbors(source)
            );

            std::vector<float> row;
            for (int destination = 0;
                destination < snapshot.vertexCount;
                ++destination)
            {
                row.push_back(
                    graph.getEdgeWeight(source, destination)
                );
            }
            snapshot.capacities.push_back(row);
        }

        return snapshot;
    }

    void requireGraphUnchanged(
        const Graph& graph,
        const GraphSnapshot& before,
        const std::string& context
    )
    {
        const GraphSnapshot after = captureGraph(graph);

        require(
            after.directed == before.directed,
            context + ": a orientacao do grafo foi modificada."
        );
        require(
            after.weighted == before.weighted,
            context + ": a ponderacao do grafo foi modificada."
        );
        requireEqual(
            after.vertexCount,
            before.vertexCount,
            context + ": quantidade de vertices"
        );
        require(
            after.neighbors == before.neighbors,
            context +
            ": vizinhos ou orientacoes foram modificados."
        );
        require(
            after.capacities == before.capacities,
            context + ": capacidades foram modificadas."
        );
    }

    template <typename GraphType>
    LocalSearchResult executeScenario(const Scenario& scenario)
    {
        GraphType graph = buildGraph<GraphType>(scenario);
        const GraphSnapshot before = captureGraph(graph);

        const LocalSearchResult result =
            LocalSearchFlow::execute(
                graph,
                scenario.source,
                scenario.sink
            );

        requireExpectedResult(result, scenario);
        requireGraphUnchanged(graph, before, scenario.name);
        return result;
    }

    template <typename Action>
    void requireInvalidArgument(
        Action action,
        const std::string& context
    )
    {
        try
        {
            action();
        }
        catch (const std::invalid_argument&)
        {
            return;
        }
        catch (const std::exception& exception)
        {
            throw std::runtime_error(
                context +
                ": esperado = std::invalid_argument, obtido = " +
                std::string(typeid(exception).name()) +
                " (\"" + exception.what() + "\")."
            );
        }
        catch (...)
        {
            throw std::runtime_error(
                context +
                ": esperado = std::invalid_argument, "
                "obtido = excecao desconhecida."
            );
        }

        throw std::runtime_error(
            context +
            ": esperado = std::invalid_argument, "
            "obtido = nenhuma excecao."
        );
    }

    const std::vector<Scenario>& requiredScenarios()
    {
        static const std::vector<Scenario> scenarios = {
            {
                "Teste 01 - Cenario obrigatorio",
                4,
                {
                    {0, 1, 10.0f},
                    {1, 3, 5.0f},
                    {2, 3, 5.0f},
                    {2, 1, 5.0f}
                },
                0, 3, 5.0, 10.0, 1
            },
            {
                "Teste 02 - Sem melhoria",
                3,
                {{0, 1, 5.0f}, {1, 2, 5.0f}},
                0, 2, 5.0, 5.0, 0
            },
            {
                "Teste 03 - Agregacao de capacidades",
                3,
                {
                    {0, 1, 4.0f},
                    {1, 0, 6.0f},
                    {1, 2, 10.0f}
                },
                0, 2, 4.0, 10.0, 1
            },
            {
                "Teste 04 - Sem caminho inicial",
                3,
                {{0, 1, 5.0f}, {2, 1, 5.0f}},
                0, 2, 0.0, 5.0, 1
            },
            {
                "Teste 05 - Destino isolado",
                3,
                {{0, 1, 5.0f}},
                0, 2, 0.0, 0.0, 0
            },
            {
                "Teste 06 - Self-loop",
                3,
                {
                    {0, 0, 7.0f},
                    {0, 1, 5.0f},
                    {1, 2, 5.0f}
                },
                0, 2, 5.0, 5.0, 0
            }
        };

        return scenarios;
    }

    const Scenario& fractionalScenario()
    {
        static const Scenario scenario{
            "Teste 12 - Capacidades fracionarias",
            3,
            {
                {0, 1, 0.25f},
                {1, 0, 0.50f},
                {1, 2, 0.75f}
            },
            0, 2, 0.25, 0.75, 1
        };
        return scenario;
    }

    const Scenario& identityScenario()
    {
        static const Scenario scenario{
            "Teste 13 - Identidade apos agregacao",
            5,
            {
                {0, 1, 4.0f},
                {1, 0, 6.0f},
                {1, 2, 10.0f},
                {2, 4, 5.0f},
                {3, 2, 5.0f},
                {3, 4, 5.0f}
            },
            0, 4, 4.0, 10.0, 2
        };
        return scenario;
    }

    std::vector<Scenario> allNumericScenarios()
    {
        std::vector<Scenario> scenarios = requiredScenarios();
        scenarios.push_back(fractionalScenario());
        scenarios.push_back(identityScenario());
        return scenarios;
    }

    template <typename GraphType>
    void testPreservation()
    {
        for (const Scenario& scenario : requiredScenarios())
        {
            GraphType graph = buildGraph<GraphType>(scenario);
            const GraphSnapshot before = captureGraph(graph);
            LocalSearchFlow::execute(
                graph,
                scenario.source,
                scenario.sink
            );
            requireGraphUnchanged(
                graph,
                before,
                "Preservacao em " + scenario.name
            );
        }
    }

    template <typename GraphType>
    void testDeterminism()
    {
        const Scenario& scenario = requiredScenarios().front();
        GraphType graph = buildGraph<GraphType>(scenario);
        const GraphSnapshot before = captureGraph(graph);

        const LocalSearchResult first =
            LocalSearchFlow::execute(graph, 0, 3);
        const LocalSearchResult second =
            LocalSearchFlow::execute(graph, 0, 3);
        const LocalSearchResult third =
            LocalSearchFlow::execute(graph, 0, 3);

        requireSameResult(second, first, "Segunda execucao");
        requireSameResult(third, first, "Terceira execucao");
        requireExpectedResult(first, scenario);
        requireGraphUnchanged(graph, before, "Determinismo");
    }

    void testRepresentationCompatibility()
    {
        for (const Scenario& scenario : allNumericScenarios())
        {
            AdjacencyListGraph listGraph =
                buildGraph<AdjacencyListGraph>(scenario);
            AdjacencyMatrixGraph matrixGraph =
                buildGraph<AdjacencyMatrixGraph>(scenario);
            const GraphSnapshot listBefore =
                captureGraph(listGraph);
            const GraphSnapshot matrixBefore =
                captureGraph(matrixGraph);

            const LocalSearchResult listResult =
                LocalSearchFlow::execute(
                    listGraph,
                    scenario.source,
                    scenario.sink
                );
            const LocalSearchResult matrixResult =
                LocalSearchFlow::execute(
                    matrixGraph,
                    scenario.source,
                    scenario.sink
                );

            requireSameResult(
                matrixResult,
                listResult,
                scenario.name + " - comparacao Matriz/Lista"
            );
            requireGraphUnchanged(
                listGraph,
                listBefore,
                scenario.name + " - Lista"
            );
            requireGraphUnchanged(
                matrixGraph,
                matrixBefore,
                scenario.name + " - Matriz"
            );
        }
    }

    template <typename GraphType>
    void testStrictImprovementAndTermination()
    {
        for (const Scenario& scenario : allNumericScenarios())
        {
            GraphType graph = buildGraph<GraphType>(scenario);
            const GraphSnapshot before = captureGraph(graph);
            const LocalSearchResult result =
                LocalSearchFlow::execute(
                    graph,
                    scenario.source,
                    scenario.sink
                );

            require(
                result.finalFlow + tolerance >= result.initialFlow,
                scenario.name +
                ": finalFlow ficou menor que initialFlow."
            );
            require(
                result.steps >= 0,
                scenario.name + ": steps ficou negativo."
            );

            if (std::fabs(
                result.finalFlow - result.initialFlow
            ) <= tolerance)
            {
                requireEqual(
                    result.steps,
                    0,
                    scenario.name +
                    ": solucao sem melhoria deve ter zero passos"
                );
            }

            if (result.steps > 0)
            {
                require(
                    result.finalFlow >
                        result.initialFlow + tolerance,
                    scenario.name +
                    ": houve passos sem melhoria estrita."
                );
            }

            requireGraphUnchanged(graph, before, scenario.name);
        }
    }

    template <typename GraphType>
    void testInvalidArguments()
    {
        const Scenario base{
            "Validacao",
            2,
            {{0, 1, 3.0f}},
            0, 1, 3.0, 3.0, 0
        };

        GraphType graph = buildGraph<GraphType>(base);
        const GraphSnapshot before = captureGraph(graph);

        requireInvalidArgument(
            [&]() { LocalSearchFlow::execute(graph, -1, 1); },
            "Fonte negativa"
        );
        requireInvalidArgument(
            [&]() { LocalSearchFlow::execute(graph, 0, 2); },
            "Destino fora do intervalo"
        );
        requireInvalidArgument(
            [&]() { LocalSearchFlow::execute(graph, 0, 0); },
            "Fonte igual ao destino"
        );
        requireGraphUnchanged(
            graph,
            before,
            "Validacao do grafo direcionado"
        );

        GraphType undirected(false, true);
        addVertices(undirected, 2);
        require(
            undirected.addEdge(0, 1, 3.0f),
            "Falha ao preparar grafo nao direcionado."
        );
        const GraphSnapshot undirectedBefore =
            captureGraph(undirected);
        requireInvalidArgument(
            [&]()
            {
                LocalSearchFlow::execute(undirected, 0, 1);
            },
            "Grafo nao direcionado"
        );
        requireGraphUnchanged(
            undirected,
            undirectedBefore,
            "Validacao do grafo nao direcionado"
        );

        GraphType unweighted(true, false);
        addVertices(unweighted, 2);
        require(
            unweighted.addEdge(0, 1),
            "Falha ao preparar grafo nao ponderado."
        );
        const GraphSnapshot unweightedBefore =
            captureGraph(unweighted);
        requireInvalidArgument(
            [&]()
            {
                LocalSearchFlow::execute(unweighted, 0, 1);
            },
            "Grafo nao ponderado"
        );
        requireGraphUnchanged(
            unweighted,
            unweightedBefore,
            "Validacao do grafo nao ponderado"
        );
    }

    template <typename GraphType>
    void registerRepresentationTests(
        TestRunner& runner,
        const std::string& representation
    )
    {
        for (const Scenario& scenario : requiredScenarios())
        {
            runner.run(
                scenario.name,
                representation,
                [&scenario]()
                {
                    executeScenario<GraphType>(scenario);
                }
            );
        }

        runner.run(
            "Teste 07 - Preservacao do grafo original",
            representation,
            []() { testPreservation<GraphType>(); }
        );
        runner.run(
            "Teste 08 - Determinismo",
            representation,
            []() { testDeterminism<GraphType>(); }
        );
        runner.run(
            "Teste 10 - Melhoria estrita e termino",
            representation,
            []()
            {
                testStrictImprovementAndTermination<GraphType>();
            }
        );
        runner.run(
            "Teste 11 - Validacao de argumentos",
            representation,
            []() { testInvalidArguments<GraphType>(); }
        );
        runner.run(
            fractionalScenario().name,
            representation,
            []()
            {
                executeScenario<GraphType>(fractionalScenario());
            }
        );
        runner.run(
            identityScenario().name,
            representation,
            []()
            {
                executeScenario<GraphType>(identityScenario());
            }
        );
    }
}

int main()
{
    std::cout << "[M3.1 - BUSCA LOCAL]\n\n";

    TestRunner runner;
    registerRepresentationTests<AdjacencyListGraph>(
        runner,
        "Lista"
    );
    registerRepresentationTests<AdjacencyMatrixGraph>(
        runner,
        "Matriz"
    );
    runner.run(
        "Teste 09 - Compatibilidade entre representacoes",
        "Lista x Matriz",
        []() { testRepresentationCompatibility(); }
    );

    return runner.finish();
}
