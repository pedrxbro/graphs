#pragma once

#include <memory>
#include <string>

#include "graph/Graph.hpp"

class GraphFileLoader
{
public:
    enum class Representation
    {
        AdjacencyMatrix,
        AdjacencyList
    };

    static std::unique_ptr<Graph> loadFromFile(
        const std::string& filePath,
        Representation representation
    );
};