#include "algorithms/mst/MstBenchmark.hpp"

#include <chrono>
#include <iomanip>
#include <ostream>
#include <utility>

MstBenchmarkResult MstBenchmark::measure(
    const Graph& graph,
    Algorithm algorithm
)
{
    // Graph must be fully loaded before this point.
    const auto start =
        std::chrono::steady_clock::now();

    MstResult mstResult = algorithm(graph);

    // Stop the clock before any printing.
    const auto end =
        std::chrono::steady_clock::now();

    MstBenchmarkResult benchmark;

    benchmark.mst = std::move(mstResult);

    benchmark.elapsedTime =
        std::chrono::duration_cast<
        std::chrono::nanoseconds
        >(end - start);

    return benchmark;
}

void MstBenchmark::print(
    std::ostream& output,
    const std::string& algorithmName,
    const std::string& representationName,
    const Graph& graph,
    const MstBenchmarkResult& benchmark
)
{
    const MstResult& result = benchmark.mst;

    const double elapsedMilliseconds =
        std::chrono::duration<double, std::milli>(
            benchmark.elapsedTime
        ).count();

    output << "\n========================================\n";
    output << "Algoritmo: " << algorithmName << "\n";
    output << "Representacao: "
        << representationName
        << "\n";

    // Keep the previous stream formatting.
    const std::ios_base::fmtflags previousFlags =
        output.flags();

    const std::streamsize previousPrecision =
        output.precision();

    output << std::fixed
        << std::setprecision(6);

    output << "Tempo de execucao: "
        << elapsedMilliseconds
        << " ms\n";

    output << std::setprecision(4);

    output << "Soma das arestas: "
        << result.totalWeight
        << "\n";

    output.flags(previousFlags);
    output.precision(previousPrecision);

    output << "Arestas escolhidas: "
        << result.edges.size()
        << "\n";

    if (!result.connected)
    {
        output << "Aviso: grafo desconexo, "
            << "resultado e uma floresta geradora\n";
    }

    const int vertexCount =
        graph.getVertexCount();

    if (vertexCount < 10)
    {
        output << "Arestas da arvore:\n";

        if (result.edges.empty())
        {
            output << "  Nenhuma aresta\n";
        }

        for (const MstEdge& edge : result.edges)
        {
            output << "  "
                << edge.source
                << " - "
                << edge.destination
                << " (peso "
                << edge.weight
                << ")\n";
        }
    }
    else
    {
        output
            << "Arestas da arvore: omitidas "
            << "(grafo com 10 ou mais vertices)\n";
    }

    output << "========================================\n";
}
