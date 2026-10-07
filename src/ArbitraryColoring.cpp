#include "algorithms/coloring/ArbitraryColoring.hpp"
#include <vector>

ColoringResult ArbitraryColoring::execute(const Graph& graph)
{
    ColoringResult result;

    const int vertexCount = graph.getVertexCount();

    // Inicializa todos os vertices sem cor (-1)
    result.colors.resize(vertexCount, -1);

    // Percorre os vertices em ordem natural: 0, 1, 2, ..., V-1
    for (int vertex = 0; vertex < vertexCount; ++vertex)
    {
        // Marca quais cores ja estao sendo usadas pelos vizinhos
        std::vector<bool> unavailableColors(vertexCount, false);

        for (int neighbor : graph.getNeighbors(vertex))
        {
            // Um laco impede uma coloracao valida
            if (neighbor == vertex)
            {
                result.colors.clear();
                return result;
            }

            int neighborColor = result.colors[neighbor];

            // Considera apenas vizinhos que ja foram coloridos
            if (neighborColor != -1)
            {
                unavailableColors[neighborColor] = true;
            }
        }

        // Procura a menor cor disponivel
        int selectedColor = 0;

        while (selectedColor < vertexCount &&
            unavailableColors[selectedColor])
        {
            ++selectedColor;
        }

        // Atribui a cor escolhida ao vertice atual
        result.colors[vertex] = selectedColor;

        // Atualiza a quantidade de cores utilizadas
        if (selectedColor + 1 > result.colorCount)
        {
            result.colorCount = selectedColor + 1;
        }
    }

    result.success = true;

    return result;
}
