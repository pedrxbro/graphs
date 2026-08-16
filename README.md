# Projeto de Grafos

Implementação desenvolvida para a disciplina de Grafos.

O projeto será desenvolvido em **C++** e terá como objetivo implementar estruturas e algoritmos relacionados à Grafos.

Inicialmente, será definida uma classe base `Graph`, que posteriormente será especializada nas seguintes representações:

* Matriz de adjacência;
* Lista de adjacência.

## Requisitos

Para compilar o projeto é necessário possuir:

* Compilador compatível com C++17;
* CMake 3.16 ou superior.

## Estrutura inicial

```text
.
├── CMakeLists.txt
├── README.md
├── include/
│   └── graph/
│       └── Graph.hpp
└── src/
    └── main.cpp
```

## Compilação

Na raiz do projeto, execute:

```bash
cmake -S . -B build
cmake --build build
```

O primeiro comando configura o projeto e cria os arquivos necessários dentro da pasta `build`.

O segundo comando realiza a compilação.

## Execução

### Windows

Dependendo do gerador utilizado pelo CMake, o executável poderá estar em:

```bash
.\build\Debug\graph_project.exe
```

ou:

```bash
.\build\graph_project.exe
```

### Linux / macOS

```bash
./build/graph_project
```