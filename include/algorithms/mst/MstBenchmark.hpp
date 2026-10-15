#pragma once

#include <chrono>
#include <iosfwd>
#include <string>

#include "algorithms/mst/MstResult.hpp"
#include "graph/Graph.hpp"

struct MstBenchmarkResult
{
    MstResult mst;
    std::chrono::nanoseconds elapsedTime{ 0 };
};

class MstBenchmark
{
public:
    using Algorithm =
        MstResult(*)(const Graph& graph);

    static MstBenchmarkResult measure(
        const Graph& graph,
        Algorithm algorithm
    );

    static void print(
        std::ostream& output,
        const std::string& algorithmName,
        const std::string& representationName,
        const Graph& graph,
        const MstBenchmarkResult& benchmark
    );
};
