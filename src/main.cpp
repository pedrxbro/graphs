#include <iostream>

#include "io/GraphFileLoader.hpp"
#include "algorithms/BreadthFirstSearch.hpp"

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

        std::cout << "BFS - Matriz: ";

        for (int vertex : matrixBfs)
        {
            std::cout << vertex << " ";
        }

        std::cout << std::endl;

        std::cout << "BFS - Lista: ";

        for (int vertex : listBfs)
        {
            std::cout << vertex << " ";
        }

        std::cout << std::endl;
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