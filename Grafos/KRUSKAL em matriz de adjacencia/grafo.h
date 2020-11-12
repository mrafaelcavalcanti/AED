#ifndef GRAFO_h
#define GRAFO_h


typedef struct grafo{
    int tamanho;
    char *mapa;
    int **matrix;
}grafo;

typedef struct aresta{
    char u;
    char v;
    int peso;
} aresta;

typedef struct lista_arestas{
    aresta arestas[40]; //valor aleatorio, precisa ser apenas maior que a quantidade de arestas
    int quantidade;
}lista_arestas;


//grafo

//cria a estrutura de um grafo a partir do tamanho
grafo * criar_grafo(int tamanho);

//adiciona uma nova aresta ao grafo
void adicionar_aresta(grafo *g, int v1, int v2, int peso);

//imprime o grafo na forma matriz de adjacencia
void imprime_Adj_matrix(grafo *g);

//encontra o indice do caractere
int indice(grafo *g, char c);

//monta um grafo a partir de um arquivo
grafo * iniciar_grafo(grafo *g);



//kruskal

//orndena a lista em ordem crescendo a partir do peso
void sort(lista_arestas *l);

//retorna a posicao de um vertice no vetor auxiliar
int buscar(int *pertence, int v);

//aplica a unicao va vb no vetor auxiliar
void aplicar_uniao(grafo *g, int *pertence, int va, int vb);

//imprime a mst
void imprime(grafo *g, lista_arestas *mst);

//algoritmo de kruskal
void kruskal(grafo *g);



#endif