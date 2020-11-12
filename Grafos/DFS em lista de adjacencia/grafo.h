#ifndef GRAFO_H
#define GRAFO_H


typedef struct no_vertice {
    char *v;
    int vertice;
    struct no_vertice * prox;
}no_vertice;


typedef struct grafo {
    int tamanho;
    char *mapa;
    int *visitado;
    no_vertice ** lista_adjacencia;
}grafo;


//grafo


grafo *criar_grafo(int tamanho);

//cria novo vertice para o grafo
no_vertice * novo_vertice(int v);

//adiciona uma nova aresta ao grafo
void adicionar_aresta(grafo *g, int v1, int v2);

//encontra o indice do caractere
int indice(grafo *g, char c);

//monta um grafo a partir de um arquivo
grafo * iniciar_grafo(grafo *g);

//imprime o grafo na forma lista de adjacencia
void imprime_grafo(grafo* g);


//DFS

//realiza a busca em profundidade em uma lista de adjacencia
void busca_profundidade(grafo *g, int vertice);

//auxiliar para a funcao de busca em profundidade
void DFS(grafo *g, char v);



#endif