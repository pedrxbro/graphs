cmake_minimum_required(VERSION 3.16)

project(Graphs VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(graphs
    src/main.cpp
)

target_include_directories(graphs PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)