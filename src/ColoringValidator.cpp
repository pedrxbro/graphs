#include "algorithms/coloring/ColoringValidator.hpp"

bool ColoringValidator::isValid(
    const Graph& graph,
    const std::vector<int>& colors
)
{
    const int vertexCount = graph.getVertexCount();

    if (colors.size() !=
        static_cast<std::size_t>(vertexCount))
    {
        return false;
    }

    for (int vertex = 0;
        vertex < vertexCount;
        ++vertex)
    {
        // Todos os vertices devem estar coloridos.
        if (colors[vertex] < 0)
        {
            return false;
        }

        for (int neighbor : graph.getNeighbors(vertex))
        {
            // Protege contra implementacoes de Graph
            // que retornem indices invalidos.
            if (neighbor < 0 ||
                neighbor >= vertexCount)
            {
                return false;
            }

            // Um laco impede uma coloracao valida.
            if (neighbor == vertex)
            {
                return false;
            }

            // Vertices adjacentes nao podem possuir
            // a mesma cor.
            if (colors[vertex] == colors[neighbor])
            {
                return false;
            }
        }
    }

    return true;
}