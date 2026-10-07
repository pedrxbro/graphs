#pragma once

#include <chrono>
#include <iosfwd>
#include <string>

#include "algorithms/coloring/ColoringResult.hpp"
#include "graph/Graph.hpp"

struct ColoringBenchmarkResult
{
    ColoringResult coloring;
    std::chrono::nanoseconds elapsedTime{ 0 };
};

class ColoringBenchmark
{
public:
    using Algorithm =
        ColoringResult(*)(const Graph& graph);

    static ColoringBenchmarkResult measure(
        const Graph& graph,
        Algorithm algorithm
    );

    static void print(
        std::ostream& output,
        const std::string& algorithmName,
        const std::string& representationName,
        const Graph& graph,
        const ColoringBenchmarkResult& benchmark
    );
};