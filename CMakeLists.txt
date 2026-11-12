
cmake_minimum_required(VERSION 3.16)

project(Graphs VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(graphs
    src/main.cpp
    src/AdjacencyMatrixGraph.cpp
    src/AdjacencyListGraph.cpp
    src/GraphFileLoader.cpp
    src/BreadthFirstSearch.cpp
    src/Dijkstra.cpp
    src/DepthFirstSearch.cpp
    src/ColoringValidator.cpp
    src/ArbitraryColoring.cpp
    src/WelshPowellColoring.cpp
    src/DSATURColoring.cpp
    src/BruteForceColoring.cpp
    src/ColoringBenchmark.cpp
    src/MstValidator.cpp
    src/PrimMst.cpp
    src/KruskalMst.cpp
    src/MstBenchmark.cpp
    src/FordFulkerson.cpp
)

target_include_directories(graphs PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

include(CTest)

if(BUILD_TESTING)
    add_executable(graphs_flow_tests
        tests/FordFulkersonTests.cpp
        src/AdjacencyMatrixGraph.cpp
        src/AdjacencyListGraph.cpp
        src/GraphFileLoader.cpp
        src/DepthFirstSearch.cpp
        src/FordFulkerson.cpp
    )

    target_include_directories(graphs_flow_tests PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )

    add_test(
        NAME ford_fulkerson_m3
        COMMAND graphs_flow_tests
            "${PROJECT_SOURCE_DIR}/tests/data/flow_clrs_6.txt"
    )
endif()
