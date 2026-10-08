#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "algorithms/BreadthFirstSearch.hpp"
#include "algorithms/DepthFirstSearch.hpp"
#include "algorithms/Dijkstra.hpp"
#include "algorithms/coloring/ArbitraryColoring.hpp"
#include "algorithms/coloring/BruteForceColoring.hpp"
#include "algorithms/coloring/ColoringBenchmark.hpp"
#include "algorithms/coloring/DSATURColoring.hpp"
#include "algorithms/coloring/WelshPowellColoring.hpp"
#include "io/GraphFileLoader.hpp"

void printVisitOrder(
    const std::string& title,
    const std::vector<int>& visitOrder
)
{
    std::cout << "\n" << title << ": ";

    if (visitOrder.empty())
    {
        std::cout << "nenhum vertice visitado";
    }
    else
    {
        for (std::size_t i = 0;
            i < visitOrder.size();
            ++i)
        {
            std::cout << visitOrder[i];

            if (i + 1 < visitOrder.size())
            {
                std::cout << " -> ";
            }
        }
    }

    std::cout << "\n";
}

void printDijkstraResult(
    const DijkstraResult& result,
    int source
)
{
    for (int destination = 0;
        destination <
        static_cast<int>(result.distances.size());
        ++destination)
    {
        std::cout << "\nVertice "
            << destination
            << "\n";

        if (std::isinf(result.distances[destination]))
        {
            std::cout << "Distancia: infinito\n";
            std::cout << "Caminho: nao alcancavel\n";
            continue;
        }

        std::cout << "Distancia: "
            << result.distances[destination]
            << "\n";

        const std::vector<int> path =
            Dijkstra::buildPath(
                result,
                source,
                destination
            );

        std::cout << "Caminho: ";

        for (std::size_t i = 0;
            i < path.size();
            ++i)
        {
            std::cout << path[i];

            if (i + 1 < path.size())
            {
                std::cout << " -> ";
            }
        }

        std::cout << "\n";
    }
}

int readSource(const Graph& graph)
{
    int source;

    std::cout << "Vertice de origem (0 a "
        << graph.getVertexCount() - 1
        << "): ";

    std::cin >> source;

    return source;
}

void showGraphs()
{
    std::unique_ptr<Graph> matrixGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::
            AdjacencyMatrix
        );

    std::unique_ptr<Graph> listGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::
            AdjacencyList
        );

    std::cout
        << "\n===== MATRIZ DE ADJACENCIA =====\n";

    matrixGraph->printGraph();

    std::cout
        << "\n===== LISTA DE ADJACENCIA =====\n";

    listGraph->printGraph();
}

void runBfs()
{
    std::unique_ptr<Graph> matrixGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::
            AdjacencyMatrix
        );

    std::unique_ptr<Graph> listGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::
            AdjacencyList
        );

    const int source = readSource(*matrixGraph);

    const std::vector<int> matrixResult =
        BreadthFirstSearch::execute(
            *matrixGraph,
            source
        );

    const std::vector<int> listResult =
        BreadthFirstSearch::execute(
            *listGraph,
            source
        );

    printVisitOrder(
        "BFS - Matriz",
        matrixResult
    );

    printVisitOrder(
        "BFS - Lista",
        listResult
    );
}

void runDfs()
{
    std::unique_ptr<Graph> matrixGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::
            AdjacencyMatrix
        );

    std::unique_ptr<Graph> listGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::
            AdjacencyList
        );

    const int source = readSource(*matrixGraph);

    const std::vector<int> matrixResult =
        DepthFirstSearch::execute(
            *matrixGraph,
            source
        );

    const std::vector<int> listResult =
        DepthFirstSearch::execute(
            *listGraph,
            source
        );

    printVisitOrder(
        "DFS - Matriz",
        matrixResult
    );

    printVisitOrder(
        "DFS - Lista",
        listResult
    );
}

void runDijkstra()
{
    std::unique_ptr<Graph> matrixGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::
            AdjacencyMatrix
        );

    std::unique_ptr<Graph> listGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::
            AdjacencyList
        );

    const int source = readSource(*matrixGraph);

    std::cout
        << "\n===== DIJKSTRA - MATRIZ =====\n";

    const DijkstraResult matrixResult =
        Dijkstra::execute(
            *matrixGraph,
            source
        );

    printDijkstraResult(
        matrixResult,
        source
    );

    std::cout
        << "\n===== DIJKSTRA - LISTA =====\n";

    const DijkstraResult listResult =
        Dijkstra::execute(
            *listGraph,
            source
        );

    printDijkstraResult(
        listResult,
        source
    );
}

bool readColoringRepresentation(
    GraphFileLoader::Representation& representation,
    std::string& representationName
)
{
    int option = -1;

    while (option != 0)
    {
        std::cout
            << "\n==============================\n";
        std::cout
            << "   REPRESENTACAO DO GRAFO\n";
        std::cout
            << "==============================\n";
        std::cout
            << "1 - Matriz de adjacencia\n";
        std::cout
            << "2 - Lista de adjacencia\n";
        std::cout
            << "0 - Voltar\n";
        std::cout
            << "Opcao: ";

        std::cin >> option;

        switch (option)
        {
        case 1:
            representation =
                GraphFileLoader::Representation::
                AdjacencyMatrix;

            representationName =
                "Matriz de adjacencia";

            return true;

        case 2:
            representation =
                GraphFileLoader::Representation::
                AdjacencyList;

            representationName =
                "Lista de adjacencia";

            return true;

        case 0:
            return false;

        default:
            std::cout
                << "\nOpcao invalida.\n";
            break;
        }
    }

    return false;
}

void runColoringAlgorithm(
    const Graph& graph,
    const std::string& representationName,
    const std::string& algorithmName,
    ColoringBenchmark::Algorithm algorithm
)
{
    const ColoringBenchmarkResult benchmark =
        ColoringBenchmark::measure(
            graph,
            algorithm
        );

    ColoringBenchmark::print(
        std::cout,
        algorithmName,
        representationName,
        graph,
        benchmark
    );
}

bool confirmBruteForceExecution(
    const Graph& graph
)
{
    char confirmation;

    std::cout
        << "\nATENCAO: a forca bruta possui "
        << "complexidade exponencial.\n";

    std::cout
        << "O grafo selecionado possui "
        << graph.getVertexCount()
        << " vertices.\n";

    std::cout
        << "Deseja continuar? (s/n): ";

    std::cin >> confirmation;

    return confirmation == 's' ||
        confirmation == 'S';
}

void runAllColoringHeuristics(
    const Graph& graph,
    const std::string& representationName
)
{
    runColoringAlgorithm(
        graph,
        representationName,
        "Heuristica Arbitraria",
        &ArbitraryColoring::execute
    );

    runColoringAlgorithm(
        graph,
        representationName,
        "Welsh-Powell",
        &WelshPowellColoring::execute
    );

    runColoringAlgorithm(
        graph,
        representationName,
        "DSATUR",
        &DSATURColoring::execute
    );
}

void printColoringMenu()
{
    std::cout
        << "\n==============================\n";
    std::cout
        << "          COLORACAO\n";
    std::cout
        << "==============================\n";
    std::cout
        << "1 - Heuristica Arbitraria\n";
    std::cout
        << "2 - Welsh-Powell\n";
    std::cout
        << "3 - DSATUR\n";
    std::cout
        << "4 - Forca Bruta\n";
    std::cout
        << "5 - Executar todas as heuristicas "
        << "para comparacao\n";
    std::cout
        << "0 - Voltar\n";
    std::cout
        << "Opcao: ";
}

void runColoring()
{
    GraphFileLoader::Representation representation =
        GraphFileLoader::Representation::
        AdjacencyMatrix;

    std::string representationName;

    if (!readColoringRepresentation(
        representation,
        representationName
    ))
    {
        return;
    }

    std::string filePath;

    std::cout
        << "\nCaminho do arquivo de coloracao\n";
    std::cout
        << "(exemplo: "
        << "coloring/small/cycle-c5.txt): ";

    std::cin >> filePath;

    // O carregamento acontece uma unica vez,
    // antes de qualquer medicao.
    std::unique_ptr<Graph> graph =
        GraphFileLoader::loadFromFile(
            filePath,
            representation
        );

    int option = -1;

    while (option != 0)
    {
        printColoringMenu();

        std::cin >> option;

        switch (option)
        {
        case 1:
            runColoringAlgorithm(
                *graph,
                representationName,
                "Heuristica Arbitraria",
                &ArbitraryColoring::execute
            );
            break;

        case 2:
            runColoringAlgorithm(
                *graph,
                representationName,
                "Welsh-Powell",
                &WelshPowellColoring::execute
            );
            break;

        case 3:
            runColoringAlgorithm(
                *graph,
                representationName,
                "DSATUR",
                &DSATURColoring::execute
            );
            break;

        case 4:
            if (confirmBruteForceExecution(*graph))
            {
                runColoringAlgorithm(
                    *graph,
                    representationName,
                    "Forca Bruta",
                    &BruteForceColoring::execute
                );
            }
            else
            {
                std::cout
                    << "\nExecucao da forca bruta "
                    << "cancelada.\n";
            }
            break;

        case 5:
            runAllColoringHeuristics(
                *graph,
                representationName
            );
            break;

        case 0:
            std::cout
                << "\nVoltando ao menu principal.\n";
            break;

        default:
            std::cout
                << "\nOpcao invalida.\n";
            break;
        }
    }
}

void printMenu()
{
    std::cout
        << "\n==============================\n";
    std::cout
        << "        PROJETO DE GRAFOS\n";
    std::cout
        << "==============================\n";
    std::cout
        << "1 - Exibir grafo nas duas representacoes\n";
    std::cout
        << "2 - Executar BFS\n";
    std::cout
        << "3 - Executar DFS\n";
    std::cout
        << "4 - Executar Dijkstra\n";
    std::cout
        << "5 - Coloracao\n";
    std::cout
        << "0 - Sair\n";
    std::cout
        << "Opcao: ";
}

int main()
{
    int option = -1;

    while (option != 0)
    {
        printMenu();

        std::cin >> option;

        try
        {
            switch (option)
            {
            case 1:
                showGraphs();
                break;

            case 2:
                runBfs();
                break;

            case 3:
                runDfs();
                break;

            case 4:
                runDijkstra();
                break;

            case 5:
                runColoring();
                break;

            case 0:
                std::cout
                    << "\nEncerrando programa.\n";
                break;

            default:
                std::cout
                    << "\nOpcao invalida.\n";
                break;
            }
        }
        catch (const std::exception& exception)
        {
            std::cout
                << "\nErro: "
                << exception.what()
                << "\n";
        }
    }

    return 0;
}