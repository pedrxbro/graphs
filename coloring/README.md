# Grafos de coloracao - M2.1

Esta pasta contem grafos nao direcionados e nao ponderados
destinados aos algoritmos de coloracao da M2.1.

## Formato

A primeira linha possui:

V A D P

Onde:

- V e a quantidade de vertices;
- A e a quantidade de arestas;
- D indica se o grafo e direcionado;
- P indica se o grafo e ponderado.

Para os testes de coloracao:

- D = 0;
- P = 0.

Cada linha seguinte possui uma aresta:

Ao Ad

Os vertices sao identificados por indices de 0 ate V - 1.

## Casos pequenos

| Arquivo | Vertices | Arestas | Numero cromatico |
|---|---:|---:|---:|
| bipartite-k3-3.txt | 6 | 9 | 2 |
| cycle-c4.txt | 4 | 4 | 2 |
| cycle-c5.txt | 5 | 5 | 3 |
| complete-k4.txt | 4 | 6 | 4 |
| different-degrees-7.txt | 7 | 9 | 3 |
| slides-welsh-powell.txt | 5 | 7 | 3 |
| slides-dsatur.txt | 5 | 6 | 3 |

## Caso medio

O arquivo medium/grid-5x5.txt representa uma grade 5 por 5.
O grafo possui 25 vertices, 40 arestas e numero cromatico 2.

Esse caso e apropriado para comparar as heuristicas.
A execucao da forca bruta nesse arquivo deve ser evitada
durante apresentacoes.

## Grafos dos slides

Nos dois arquivos dos slides foi utilizado o mapeamento:

- A = 0;
- B = 1;
- C = 2;
- D = 3;
- E = 4.

Os slides de Welsh-Powell e DSATUR utilizam grafos diferentes,
por isso cada exemplo possui seu proprio arquivo.

## Compatibilidade com a M1

O graph.txt da raiz nao deve ser substituido. Ele continua sendo
o arquivo ponderado utilizado por BFS, DFS e Dijkstra.

Os grafos desta pasta devem ser carregados por caminho explicito
quando a interface de coloracao for integrada.