# Projeto de Grafos

Trabalho desenvolvido em **C++17** para a disciplina de Grafos.

O projeto implementa duas representacoes de grafo:

- matriz de adjacencia;
- lista de adjacencia.

As duas representacoes seguem a interface `Graph`, permitindo executar
os algoritmos sem dependencia da representacao concreta.

## Funcionalidades

### M1

- insercao e remocao de vertices;
- insercao e remocao de arestas;
- verificacao de arestas;
- consulta de peso;
- consulta de vizinhos;
- leitura de grafos por arquivo;
- Busca em Largura (BFS);
- Busca em Profundidade (DFS);
- Dijkstra.

### M2.1 - Coloracao de grafos

- Heuristica Arbitraria;
- Welsh-Powell;
- DSATUR;
- Forca Bruta;
- validacao das coloracoes;
- execucao em matriz ou lista de adjacencia;
- medicao do tempo de execucao;
- exibicao do numero de cores utilizadas;
- exibicao da cor de cada vertice para grafos com menos de 10 vertices.

A Forca Bruta testa quantidades crescentes de cores, iniciando em duas,
ate encontrar uma coloracao valida. Por possuir crescimento exponencial,
sua execucao deve ser reservada para grafos pequenos.

## Formato dos arquivos

A primeira linha do arquivo possui:

```text
V A D P
```

Onde:

- `V`: quantidade de vertices;
- `A`: quantidade de arestas;
- `D`: indica se o grafo e direcionado (`0` ou `1`);
- `P`: indica se o grafo e ponderado (`0` ou `1`).

As linhas seguintes representam as arestas.

Para grafos nao ponderados:

```text
Ao Ad
```

Para grafos ponderados:

```text
Ao Ad Ap
```

Onde:

- `Ao`: vertice de origem;
- `Ad`: vertice de destino;
- `Ap`: peso da aresta.

Os vertices sao identificados por indices de `0` ate `V - 1`.

## Arquivos de teste

O arquivo `graph.txt`, localizado na raiz do projeto, permanece como
exemplo ponderado utilizado nas funcionalidades da M1.

Os grafos da M2.1 estao organizados da seguinte forma:

```text
coloring/
|-- small/
|-- medium/
`-- large/
```

Alguns exemplos disponiveis:

```text
coloring/small/bipartite-k3-3.txt
coloring/small/complete-k4.txt
coloring/small/cycle-c4.txt
coloring/small/cycle-c5.txt
coloring/small/different-degrees-7.txt
coloring/small/slides-dsatur.txt
coloring/small/slides-welsh-powell.txt
coloring/medium/grid-5x5.txt
```

Os arquivos de coloracao representam grafos nao direcionados e nao
ponderados. Portanto, utilizam:

```text
D = 0
P = 0
```

Mais detalhes sobre os grafos de teste e seus numeros cromaticos
esperados estao disponiveis em `coloring/README.md`.

## Compilacao

A partir da raiz do projeto, execute:

```bash
cmake -S . -B build
cmake --build build --config Release
```

No Windows com Visual Studio, o executavel normalmente sera gerado em:

```text
build\Release\graphs.exe
```

Dependendo do gerador utilizado, tambem pode ser gerado em:

```text
build\graphs.exe
```

## Execucao

Ao executar o programa, o menu principal permite:

```text
1 - Exibir grafo nas duas representacoes
2 - Executar BFS
3 - Executar DFS
4 - Executar Dijkstra
5 - Coloracao
0 - Sair
```

A opcao de coloracao permite escolher entre:

```text
1 - Matriz de adjacencia
2 - Lista de adjacencia
0 - Voltar
```

Depois de selecionar a representacao, o usuario deve informar o caminho
do arquivo que contem o grafo.

O submenu de coloracao oferece:

```text
1 - Heuristica Arbitraria
2 - Welsh-Powell
3 - DSATUR
4 - Forca Bruta
5 - Executar todas as heuristicas para comparacao
0 - Voltar
```

A opcao de comparacao executa as heuristicas Arbitraria, Welsh-Powell e
DSATUR. A Forca Bruta deve ser selecionada separadamente, pois sua
complexidade torna a execucao inviavel para grafos grandes.

## Caminho dos arquivos

O caminho informado pelo usuario e interpretado em relacao ao diretorio
de execucao do programa.

Caso o programa seja iniciado pelo Visual Studio a partir de um
diretorio de build, um caminho relativo como:

```text
coloring/small/cycle-c5.txt
```

pode nao ser encontrado.

Nesse caso, deve ser informado o caminho absoluto do arquivo, por
exemplo:

```text
C:\caminho\do\projeto\coloring\small\cycle-c5.txt
```

Tambem e possivel ajustar o diretorio de trabalho da configuracao de
execucao para apontar para a raiz do projeto.

## Resultados da coloracao

Para cada algoritmo de coloracao sao apresentados:

- nome do algoritmo;
- tempo de execucao;
- numero de cores utilizadas;
- estado da solucao.

Para grafos com menos de 10 vertices, tambem sao apresentados:

- identificador do vertice;
- cor atribuida ao vertice.

A medicao considera somente a execucao do algoritmo. O carregamento do
arquivo e a impressao dos resultados nao fazem parte do tempo medido.

## Algoritmos de coloracao

### Heuristica Arbitraria

Percorre os vertices em sua ordem natural e atribui a menor cor que nao
esteja sendo utilizada por um vizinho ja colorido.

### Welsh-Powell

Ordena os vertices por grau decrescente. Em caso de empate, o vertice
de menor indice possui prioridade.

Em cada passada, uma nova cor e atribuida ao maior numero possivel de
vertices nao adjacentes entre si.

### DSATUR

Seleciona os vertices de acordo com o grau de saturacao, correspondente
a quantidade de cores diferentes presentes nos vizinhos ja coloridos.

Os criterios de escolha sao, nesta ordem:

1. maior grau de saturacao;
2. maior grau do vertice;
3. menor indice do vertice.

O vertice selecionado recebe a menor cor disponivel.

### Forca Bruta

Testa todas as combinacoes necessarias, iniciando com duas cores.

Se nenhuma coloracao valida for encontrada, tenta tres cores e continua
aumentando a quantidade ate encontrar uma solucao.

A primeira quantidade de cores que produz uma solucao valida corresponde
ao numero cromatico encontrado pelo algoritmo.

Devido ao crescimento exponencial da quantidade de combinacoes, esse
algoritmo deve ser utilizado apenas em grafos pequenos.

## Representacoes

Todos os algoritmos de coloracao recebem a interface `Graph`.

Dessa forma, os mesmos algoritmos podem ser executados utilizando:

- `AdjacencyMatrixGraph`;
- `AdjacencyListGraph`.

Nao existem implementacoes especificas dos algoritmos de coloracao para
cada representacao.