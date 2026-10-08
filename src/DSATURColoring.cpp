#include "algorithms/coloring/DSATURColoring.hpp"

#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <vector>

ColoringResult DSATURColoring::execute(const Graph& graph)
{
    ColoringResult result;

    const int vertexCount = graph.getVertexCount();

    // Todos os vertices comecam sem cor.
    result.colors.resize(vertexCount, -1);

    // O grafo vazio possui coloracao valida com zero cores.
    if (vertexCount == 0)
    {
        result.success = true;
        return result;
    }

    // Guarda os vizinhos para evitar chamadas repetidas
    // a graph.getNeighbors().
    std::vector<std::vector<int>> neighbors(vertexCount);

    // Grau comum de cada vertice.
    std::vector<int> degrees(vertexCount, 0);

    // Grau de saturacao de cada vertice.
    std::vector<int> saturationDegrees(vertexCount, 0);

    // Conjunto de cores diferentes presentes nos vizinhos
    // ja coloridos de cada vertice.
    std::vector<std::unordered_set<int>> neighborColorSets(
        vertexCount
    );

    // Ordem inicial por grau decrescente.
    std::vector<int> orderedVertices(vertexCount);

    std::iota(
        orderedVertices.begin(),
        orderedVertices.end(),
        0
    );

    // Obtem os vizinhos, verifica lacos e calcula os graus.
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

    // Ordena por grau decrescente.
    // Em caso de empate, o menor indice vem primeiro.
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

    // O primeiro vertice e o de maior grau.
    const int initialVertex = orderedVertices.front();

    result.colors[initialVertex] = 0;
    result.colorCount = 1;

    int coloredCount = 1;

    // Atualiza a saturacao dos vizinhos do primeiro vertice.
    for (int neighbor : neighbors[initialVertex])
    {
        if (result.colors[neighbor] == -1)
        {
            const bool inserted =
                neighborColorSets[neighbor].insert(0).second;

            if (inserted)
            {
                ++saturationDegrees[neighbor];
            }
        }
    }

    while (coloredCount < vertexCount)
    {
        int selectedVertex = -1;

        // Seleciona o melhor vertice ainda sem cor:
        // 1. maior saturacao;
        // 2. maior grau;
        // 3. menor indice.
        for (int vertex = 0; vertex < vertexCount; ++vertex)
        {
            if (result.colors[vertex] != -1)
            {
                continue;
            }

            if (selectedVertex == -1 ||
                saturationDegrees[vertex] >
                saturationDegrees[selectedVertex] ||
                (saturationDegrees[vertex] ==
                    saturationDegrees[selectedVertex] &&
                    degrees[vertex] >
                    degrees[selectedVertex]) ||
                (saturationDegrees[vertex] ==
                    saturationDegrees[selectedVertex] &&
                    degrees[vertex] ==
                    degrees[selectedVertex] &&
                    vertex < selectedVertex))
            {
                selectedVertex = vertex;
            }
        }

        if (selectedVertex == -1)
        {
            return ColoringResult{};
        }

        // Marca as cores que ja aparecem nos vizinhos
        // do vertice selecionado.
        std::vector<bool> unavailableColors(
            vertexCount,
            false
        );

        for (int neighbor : neighbors[selectedVertex])
        {
            const int neighborColor =
                result.colors[neighbor];

            if (neighborColor != -1)
            {
                unavailableColors[neighborColor] = true;
            }
        }

        // Escolhe a menor cor nao utilizada pelos vizinhos.
        int selectedColor = 0;

        while (selectedColor < vertexCount &&
            unavailableColors[selectedColor])
        {
            ++selectedColor;
        }

        result.colors[selectedVertex] = selectedColor;
        ++coloredCount;

        result.colorCount = std::max(
            result.colorCount,
            selectedColor + 1
        );

        // A nova cor afeta somente a saturacao dos
        // vizinhos que ainda nao foram coloridos.
        for (int neighbor : neighbors[selectedVertex])
        {
            if (result.colors[neighbor] != -1)
            {
                continue;
            }

            const bool inserted =
                neighborColorSets[neighbor]
                .insert(selectedColor)
                .second;

            // A saturacao aumenta apenas quando a cor
            // ainda nao estava presente nos vizinhos.
            if (inserted)
            {
                ++saturationDegrees[neighbor];
            }
        }
    }

    result.success = true;

    return result;
}