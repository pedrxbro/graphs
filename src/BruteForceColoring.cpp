#include "algorithms/coloring/BruteForceColoring.hpp"

#include <vector>

#include "algorithms/coloring/ColoringValidator.hpp"

namespace
{
    bool advanceCombination(
        std::vector<int>& colors,
        int availableColorCount
    )
    {
        // O vetor de cores funciona como um contador
        // na base availableColorCount.
        for (int vertex =
            static_cast<int>(colors.size()) - 1;
            vertex >= 0;
            --vertex)
        {
            ++colors[vertex];

            // Ainda nao ocorreu estouro nesta posicao.
            if (colors[vertex] < availableColorCount)
            {
                return true;
            }

            // Reinicia a posicao e propaga o incremento
            // para o vertice anterior.
            colors[vertex] = 0;
        }

        // Todas as combinacoes foram geradas.
        return false;
    }
}

ColoringResult BruteForceColoring::execute(
    const Graph& graph
)
{
    const int vertexCount = graph.getVertexCount();

    // O grafo vazio possui coloracao valida
    // utilizando zero cores.
    if (vertexCount == 0)
    {
        ColoringResult emptyResult;
        emptyResult.success = true;

        return emptyResult;
    }

    bool hasAnyEdge = false;

    // Detecta casos que podem ser resolvidos antes
    // da enumeracao das combinacoes.
    for (int vertex = 0; vertex < vertexCount; ++vertex)
    {
        const std::vector<int> neighbors =
            graph.getNeighbors(vertex);

        for (int neighbor : neighbors)
        {
            // Um laco torna impossivel uma coloracao valida.
            if (neighbor == vertex)
            {
                return ColoringResult{};
            }

            hasAnyEdge = true;
        }
    }

    // Um grafo nao vazio e sem arestas possui
    // numero cromatico igual a 1.
    if (!hasAnyEdge)
    {
        ColoringResult result;

        result.colors.resize(vertexCount, 0);
        result.colorCount = 1;
        result.success = true;

        return result;
    }

    // Para grafos com pelo menos uma aresta,
    // o numero cromatico e no minimo 2.
    for (int availableColorCount = 2;
        availableColorCount <= vertexCount;
        ++availableColorCount)
    {
        // A primeira combinacao e:
        // 0, 0, 0, ..., 0.
        std::vector<int> colors(vertexCount, 0);

        bool hasCombination = true;

        while (hasCombination)
        {
            if (ColoringValidator::isValid(
                graph,
                colors
            ))
            {
                ColoringResult result;

                result.colors = colors;
                result.colorCount = availableColorCount;
                result.success = true;

                return result;
            }

            hasCombination = advanceCombination(
                colors,
                availableColorCount
            );
        }
    }

    // Todo grafo sem lacos pode ser colorido com,
    // no maximo, uma cor por vertice. Este retorno
    // existe como protecao para entradas inconsistentes.
    return ColoringResult{};
}