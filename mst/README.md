# Grafos de teste - Arvore Geradora Minima

Todos os arquivos sao **nao direcionados e ponderados** (`D = 0`, `P = 1`).
Pesos sao sempre maiores que zero (a matriz usa `0` como "sem aresta").

Os valores esperados foram calculados por uma implementacao
independente e conferidos com Prim e Kruskal, em matriz e em lista.

| Arquivo | V | A | Peso da AGM | Observacao |
| --- | --- | --- | --- | --- |
| small/clrs-9.txt | 9 | 14 | 37 | exemplo classico do CLRS |
| small/triangle-k3.txt | 3 | 3 | 3 | menor caso com ciclo |
| small/single-vertex.txt | 1 | 0 | 0 | caso limite |
| small/isolated-vertex.txt | 4 | 2 | 5 | vertice 3 isolado (floresta) |
| small/disconnected-6.txt | 6 | 4 | 17 | 2 componentes (floresta) |
| medium/grid-5x5.txt | 25 | 40 | 128 | grade com pesos variados |
| medium/equal-weights-12.txt | 12 | 44 | 55 | todas as arestas com peso 5 (empates) |
| large/sparse-2000.txt | 2000 | 6000 | 405380 | grafo esparso conexo |
| large/dense-300.txt | 300 | 26874 | 2143 | grafo denso (~60% das arestas) |

Nos grafos desconexos o programa exibe um aviso e o resultado e uma
floresta geradora (V - componentes arestas).
