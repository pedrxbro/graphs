# Grafos grandes de coloracao

Esta pasta e reservada para grafos grandes, nao direcionados
e nao ponderados, utilizados nos testes de desempenho da M2.1.

## Formato

Todos os arquivos devem utilizar:

V A 0 0
Ao Ad
Ao Ad
...

## Convencao de nomes

Utilizar nomes que indiquem a quantidade de vertices e arestas:

large-v1000-e5000-01.txt
large-v5000-e25000-01.txt
large-v10000-e50000-01.txt

## Cuidados

- Manter apenas uma ocorrencia de cada aresta.
- Nao incluir lacos.
- Usar vertices entre 0 e V - 1.
- Conferir se a quantidade de linhas de arestas corresponde a A.
- Nao adicionar pesos.
- Nao substituir o graph.txt da raiz.
- Registrar a origem ou o metodo de geracao do grafo.
- Registrar o numero cromatico somente quando ele for conhecido.

Os grafos grandes devem ser utilizados somente com as heuristicas
arbitraria, Welsh-Powell e DSATUR.

A forca bruta nao deve ser executada nesses arquivos, pois seu
tempo cresce exponencialmente com a quantidade de vertices.