#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "algorithms/BreadthFirstSearch.hpp"
#include "algorithms/DepthFirstSearch.hpp"
#include "algorithms/Dijkstra.hpp"
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
        for (std::size_t i = 0; i < visitOrder.size(); ++i)
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
         destination < static_cast<int>(result.distances.size());
         ++destination)
    {
        std::cout << "\nVertice " << destination << "\n";

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
            Dijkstra::buildPath(result, source, destination);

        std::cout << "Caminho: ";

        for (std::size_t i = 0; i < path.size(); ++i)
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
            GraphFileLoader::Representation::AdjacencyMatrix
        );

    std::unique_ptr<Graph> listGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::AdjacencyList
        );

    std::cout << "\n===== MATRIZ DE ADJACENCIA =====\n";
    matrixGraph->printGraph();

    std::cout << "\n===== LISTA DE ADJACENCIA =====\n";
    listGraph->printGraph();
}

void runBfs()
{
    std::unique_ptr<Graph> matrixGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::AdjacencyMatrix
        );

    std::unique_ptr<Graph> listGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::AdjacencyList
        );

    const int source = readSource(*matrixGraph);

    const std::vector<int> matrixResult =
        BreadthFirstSearch::execute(*matrixGraph, source);

    const std::vector<int> listResult =
        BreadthFirstSearch::execute(*listGraph, source);

    printVisitOrder("BFS - Matriz", matrixResult);
    printVisitOrder("BFS - Lista", listResult);
}

void runDfs()
{
    std::unique_ptr<Graph> matrixGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::AdjacencyMatrix
        );

    std::unique_ptr<Graph> listGraph =
        GraphFileLoader::loadFromFile(
            "graph.txt",
            GraphFileLoader::Representation::AdjacencyList
        );

    const int source = readSource(*matrixGraph);

    const std::vector<int> matrixResult =
        DepthFirstSearch::execute(*matrixGraph, source);

    const std::vector<int> listResult =
        DepthFirstSearch::execute(*listGraph, source);

    printVisitOrder("DFS - Matriz", matrixResult);
    printVisitOrder("DFS - Lista", listResult);
}

void runDijkstra()
{
    std::unique_ptr<Graph> matrixGraph =
        GraphFileLoader::loadFromFile(
            "dijkstra.txt",
            GraphFileLoader::Representation::AdjacencyMatrix
        );

    std::unique_ptr<Graph> listGraph =
        GraphFileLoader::loadFromFile(
            "dijkstra.txt",
            GraphFileLoader::Representation::AdjacencyList
        );

    const int source = readSource(*matrixGraph);

    std::cout << "\n===== DIJKSTRA - MATRIZ =====\n";
    const DijkstraResult matrixResult =
        Dijkstra::execute(*matrixGraph, source);
    printDijkstraResult(matrixResult, source);

    std::cout << "\n===== DIJKSTRA - LISTA =====\n";
    const DijkstraResult listResult =
        Dijkstra::execute(*listGraph, source);
    printDijkstraResult(listResult, source);
}

void printMenu()
{
    std::cout << "\n==============================\n";
    std::cout << "        PROJETO DE GRAFOS\n";
    std::cout << "==============================\n";
    std::cout << "1 - Exibir grafo nas duas representacoes\n";
    std::cout << "2 - Executar BFS\n";
    std::cout << "3 - Executar DFS\n";
    std::cout << "4 - Executar Dijkstra\n";
    std::cout << "0 - Sair\n";
    std::cout << "Opcao: ";
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

                case 0:
                    std::cout << "\nEncerrando programa.\n";
                    break;

                default:
                    std::cout << "\nOpcao invalida.\n";
                    break;
            }
        }
        catch (const std::exception& exception)
        {
            std::cout << "\nErro: "
                      << exception.what()
                      << "\n";
        }
    }

    return 0;
}
