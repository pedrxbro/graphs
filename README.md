# Projeto de Grafos

Trabalho desenvolvido em **C++17** para a disciplina de Grafos.

O projeto implementa duas representacoes de grafo:

- matriz de adjacencia;
- lista de adjacencia.

As duas representacoes seguem a mesma interface `Graph`, permitindo que os algoritmos sejam executados com o mesmo codigo.

## Funcionalidades

- insercao e remocao de vertices;
- insercao e remocao de arestas;
- verificacao de arestas;
- consulta de peso;
- consulta de vizinhos;
- leitura de grafos por arquivo;
- Busca em Largura (BFS);
- Busca em Profundidade (DFS);
- Dijkstra.

## Formato dos arquivos

```text
V A D P
Ao Ad Ap
...
```

Onde:

- `V`: numero de vertices;
- `A`: numero de arestas;
- `D`: direcionado (0 ou 1);
- `P`: ponderado (0 ou 1);
- `Ao`: vertice de origem;
- `Ad`: vertice de destino;
- `Ap`: peso, apenas para grafos ponderados.

O arquivo `graph.txt` e usado em todas as demonstracoes do menu, incluindo BFS, DFS e Dijkstra.

## Compilacao

```bash
cmake -S . -B build
cmake --build build
```

## Execucao

No Windows, conforme o gerador utilizado:

```bash
build\graphs.exe
```

ou

```bash
build\Debug\graphs.exe
```

Ao executar, o programa apresenta um menu para demonstrar as duas representacoes e os algoritmos BFS, DFS e Dijkstra.
