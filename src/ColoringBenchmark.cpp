#include "algorithms/coloring/ColoringBenchmark.hpp"

#include <chrono>
#include <iomanip>
#include <ostream>
#include <utility>

ColoringBenchmarkResult ColoringBenchmark::measure(
    const Graph& graph,
    Algorithm algorithm
)
{
    // O grafo deve estar completamente carregado
    // antes do inicio desta medicao.
    const auto start =
        std::chrono::steady_clock::now();

    ColoringResult coloringResult =
        algorithm(graph);

    // O cronometro termina antes de qualquer validacao,
    // comparacao ou impressao.
    const auto end =
        std::chrono::steady_clock::now();

    ColoringBenchmarkResult benchmark;

    benchmark.coloring = std::move(coloringResult);

    benchmark.elapsedTime =
        std::chrono::duration_cast<
        std::chrono::nanoseconds
        >(end - start);

    return benchmark;
}

void ColoringBenchmark::print(
    std::ostream& output,
    const std::string& algorithmName,
    const std::string& representationName,
    const Graph& graph,
    const ColoringBenchmarkResult& benchmark
)
{
    const ColoringResult& result =
        benchmark.coloring;

    const double elapsedMilliseconds =
        std::chrono::duration<double, std::milli>(
            benchmark.elapsedTime
        ).count();

    output << "\n========================================\n";
    output << "Algoritmo: " << algorithmName << "\n";
    output << "Representacao: "
        << representationName
        << "\n";

    // Preserva a formatacao anterior do stream.
    const std::ios_base::fmtflags previousFlags =
        output.flags();

    const std::streamsize previousPrecision =
        output.precision();

    output << std::fixed
        << std::setprecision(6);

    output << "Tempo de execucao: "
        << elapsedMilliseconds
        << " ms\n";

    output.flags(previousFlags);
    output.precision(previousPrecision);

    output << "Resultado: "
        << (result.success ? "sucesso" : "falha")
        << "\n";

    output << "Numero de cores: "
        << result.colorCount
        << "\n";

    if (!result.success)
    {
        output << "========================================\n";
        return;
    }

    const int vertexCount =
        graph.getVertexCount();

    if (vertexCount < 10)
    {
        output << "Vertices e cores:\n";

        if (vertexCount == 0)
        {
            output << "  Grafo vazio\n";
        }
        else if (result.colors.size() !=
            static_cast<std::size_t>(vertexCount))
        {
            output << "  Resultado inconsistente\n";
        }
        else
        {
            for (int vertex = 0;
                vertex < vertexCount;
                ++vertex)
            {
                output << "  Vertice "
                    << vertex
                    << ": cor "
                    << result.colors[vertex]
                    << "\n";
            }
        }
    }
    else
    {
        output
            << "Vertices e cores: omitidos "
            << "(grafo com 10 ou mais vertices)\n";
    }

    output << "========================================\n";
}