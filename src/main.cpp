#include <iostream>

#include "io/GraphFileLoader.hpp"

int main()
{
    try
    {
        std::unique_ptr<Graph> matrixGraph =
            GraphFileLoader::loadFromFile(
                "graph.txt",
                GraphFileLoader::Representation::AdjacencyMatrix
            );

        std::cout << "Matriz de adjacencia:" << std::endl;
        matrixGraph->printGraph();

        std::cout << std::endl;

        std::unique_ptr<Graph> listGraph =
            GraphFileLoader::loadFromFile(
                "graph.txt",
                GraphFileLoader::Representation::AdjacencyList
            );

        std::cout << "Lista de adjacencia:" << std::endl;
        listGraph->printGraph();
    }
    catch (const std::exception& exception)
    {
        std::cout << "Erro: "
            << exception.what()
            << std::endl;
    }

    return 0;
}