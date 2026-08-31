#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "io/GraphFileLoader.hpp"
#include "algorithms/BreadthFirstSearch.hpp"
#include "algorithms/DepthFirstSearch.hpp"

void printQueue(std::queue<int> queue)
{
    std::cout << "[ ";

    while (!queue.empty())
    {
        std::cout << queue.front() << " ";
        queue.pop();
    }

    std::cout << "]";
}

void showBfsStepByStep(const Graph& graph, int source)
{
    int vertexCount = graph.getVertexCount();

    if (source < 0 || source >= vertexCount)
    {
        std::cout << "Vertice de origem invalido." << std::endl;
        return;
    }

    std::vector<bool> visited(vertexCount, false);
    std::vector<int> visitOrder;
    std::queue<int> queue;

    visited[source] = true;
    queue.push(source);

    int step = 1;

    while (!queue.empty())
    {
        std::cout << "\n==============================" << std::endl;
        std::cout << "Passo " << step << std::endl;
        std::cout << "==============================" << std::endl;

        std::cout << "Fila antes: ";
        printQueue(queue);
        std::cout << std::endl;

        int currentVertex = queue.front();
        queue.pop();

        std::cout << "Vertice atual: "
            << currentVertex
            << std::endl;

        visitOrder.push_back(currentVertex);

        std::vector<int> neighbors =
            graph.getNeighbors(currentVertex);

        std::cout << "Vizinhos: [ ";

        for (int neighbor : neighbors)
        {
            std::cout << neighbor << " ";
        }

        std::cout << "]" << std::endl;

        for (int neighbor : neighbors)
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                queue.push(neighbor);

                std::cout
                    << "Adicionando "
                    << neighbor
                    << " na fila."
                    << std::endl;
            }
        }

        std::cout << "Fila depois: ";
        printQueue(queue);
        std::cout << std::endl;

        std::cout << "Visitados: [ ";

        for (int vertex : visitOrder)
        {
            std::cout << vertex << " ";
        }

        std::cout << "]" << std::endl;

        std::cout
            << "\nPressione ENTER para continuar...";

        std::cin.get();

        step++;
    }

    std::cout << "\nBFS concluida." << std::endl;

    std::cout << "Ordem final: ";

    for (int vertex : visitOrder)
    {
        std::cout << vertex << " ";
    }

    std::cout << std::endl;
}

void printVisitOrder(
    const std::string& label,
    const std::vector<int>& visitOrder
)
{
    std::cout << label << ": ";

    for (int vertex : visitOrder)
    {
        std::cout << vertex << " ";
    }

    std::cout << std::endl;
}

int main()
{
    try
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

        std::vector<int> matrixBfs =
            BreadthFirstSearch::execute(*matrixGraph, 0);

        std::vector<int> listBfs =
            BreadthFirstSearch::execute(*listGraph, 0);

        std::vector<int> matrixDfs =
            DepthFirstSearch::execute(*matrixGraph, 0);

        std::vector<int> listDfs =
            DepthFirstSearch::execute(*listGraph, 0);

        printVisitOrder("BFS - Matriz", matrixBfs);
        printVisitOrder("BFS - Lista", listBfs);
        printVisitOrder("DFS - Matriz", matrixDfs);
        printVisitOrder("DFS - Lista", listDfs);
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "Erro: "
            << exception.what()
            << std::endl;
    }

    return 0;
}
