#include "algorithms/coloring/WelshPowellColoring.hpp"

#include <algorithm>
#include <numeric>
#include <vector>

ColoringResult WelshPowellColoring::execute(const Graph& graph)
{
    ColoringResult result;

    const int vertexCount = graph.getVertexCount();

    // Todos os vertices comecam sem cor.
    result.colors.resize(vertexCount, -1);

    // Guarda os vizinhos para evitar chamadas repetidas a getNeighbors.
    std::vector<std::vector<int>> neighbors(vertexCount);

    // Grau de cada vertice.
    std::vector<int> degrees(vertexCount, 0);

    // Ordem em que os vertices serao percorridos.
    std::vector<int> orderedVertices(vertexCount);

    std::iota(
        orderedVertices.begin(),
        orderedVertices.end(),
        0
    );

    // Obtem os vizinhos e calcula os graus.
    for (int vertex = 0; vertex < vertexCount; ++vertex)
    {
        neighbors[vertex] = graph.getNeighbors(vertex);

        for (int neighbor : neighbors[vertex])
        {
            // Um grafo com laco nao possui coloracao valida.
            if (neighbor == vertex)
            {
                return ColoringResult{};
            }
        }

        degrees[vertex] =
            static_cast<int>(neighbors[vertex].size());
    }

    // Maior grau primeiro.
    // Em caso de empate, menor indice primeiro.
    std::sort(
        orderedVertices.begin(),
        orderedVertices.end(),
        [&degrees](int left, int right)
        {
            if (degrees[left] != degrees[right])
            {
                return degrees[left] > degrees[right];
            }

            return left < right;
        }
    );

    int uncoloredCount = vertexCount;
    int currentColor = 0;

    // Cada repeticao utiliza exatamente uma nova cor.
    while (uncoloredCount > 0)
    {
        bool assignedInCurrentPass = false;

        // Percorre os vertices na ordem definida pelo grau.
        for (int vertex : orderedVertices)
        {
            if (result.colors[vertex] != -1)
            {
                continue;
            }

            bool canUseCurrentColor = true;

            // A cor atual so pode ser aplicada quando nenhum
            // vizinho ja possui essa mesma cor.
            for (int neighbor : neighbors[vertex])
            {
                if (result.colors[neighbor] == currentColor)
                {
                    canUseCurrentColor = false;
                    break;
                }
            }

            if (canUseCurrentColor)
            {
                result.colors[vertex] = currentColor;
                --uncoloredCount;
                assignedInCurrentPass = true;
            }
        }

        // Protecao contra uma repeticao sem progresso.
        if (!assignedInCurrentPass)
        {
            return ColoringResult{};
        }

        ++currentColor;
    }

    result.colorCount = currentColor;
    result.success = true;

    return result;
}