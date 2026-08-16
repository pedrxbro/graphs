#include <iostream>
#include <type_traits>

#include "graph/Graph.hpp"

int main()
{
    static_assert(
        std::is_abstract_v<Graph>,
        "Graph precisa ser uma classe abstrata."
        );

    std::cout << "Projeto inicial de Grafos." << std::endl;

    return 0;
}