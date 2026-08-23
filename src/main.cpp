#include <iostream>
#include <type_traits>

#include "graph/Graph.hpp"
#include "graph/AdjacencyMatrixGraph.hpp"
#include "graph/AdjacencyListGraph.hpp"

int main()
{

    // Não direcionado e não ponderado
    AdjacencyMatrixGraph adjacencyMatrixGraph(false, false);

    adjacencyMatrixGraph.addVertex();
    adjacencyMatrixGraph.addVertex();
    adjacencyMatrixGraph.addVertex();
    adjacencyMatrixGraph.addVertex();

    adjacencyMatrixGraph.addEdge(0, 1);
    adjacencyMatrixGraph.addEdge(0, 2);
    adjacencyMatrixGraph.addEdge(0, 3);
    adjacencyMatrixGraph.addEdge(1, 3);

    std::cout << "Matriz de adjacencia: " << std::endl;
    adjacencyMatrixGraph.printGraph();

    std::cout << std::endl;

    std::cout << "Vizinhos do vertice 1: ";
    for (int neighbor : adjacencyMatrixGraph.getNeighbors(1))
    {
        std::cout << neighbor << " ";
    }

    std::cout << std::endl;
    std::cout << std::endl;


    // Não direcionado e não ponderado
    AdjacencyListGraph adjacencyListGraph(false, false);

    adjacencyListGraph.addVertex();
    adjacencyListGraph.addVertex();
    adjacencyListGraph.addVertex();
    adjacencyListGraph.addVertex();

    adjacencyListGraph.addEdge(0, 1);
    adjacencyListGraph.addEdge(0, 2);
    adjacencyListGraph.addEdge(0, 3);
    adjacencyListGraph.addEdge(1, 3);

    std::cout << "Lista de adjacencia: " << std::endl;
    adjacencyListGraph.printGraph();

    std::cout << std::endl;

    std::cout << "Vizinhos do vertice 1: ";
    for (int neighbor : adjacencyListGraph.getNeighbors(1))
    {
        std::cout << neighbor << " ";
    }

    std::cout << std::endl;

    return 0;
}