#include <iostream>
#include <type_traits>

#include "graph/Graph.hpp"
#include "graph/AdjacencyMatrixGraph.hpp"

int main()
{
    static_assert(
        std::is_abstract_v<Graph>,
        "Graph precisa ser uma classe abstrata."
        );

    static_assert(
        std::is_base_of_v<Graph, AdjacencyMatrixGraph>,
        "AdjacencyMatrixGraph precisa herdar de Graph"
        );

    std::cout << "Projeto compilado com sucesso." << std::endl;

    return 0;
}