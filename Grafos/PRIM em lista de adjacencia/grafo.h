#ifndef GRAFO_H
#define GRAFO_H


typedef struct no_heap{
    int v;
    int chave;
}no_heap;

typedef struct heap{
    int tamanho_heap;
    int capacidade;
    int *posicao;
    no_heap ** matrix;
}heap;


typedef struct no_vertice {
    char *v;
    int vertice;
    int peso;
    struct no_vertice * prox;
}no_vertice;


typedef struct grafo {
    int tamanho;
    char *mapa;
    int *visitado;
    no_vertice ** lista_adjacencia;
}grafo;


//grafo

//cria a estrutura de um grafo a partir do tamanho
grafo *criar_grafo(int tamanho);

//cria novo vertice para o grafo
no_vertice * novo_vertice(int v, int peso);

//adiciona uma nova aresta ao grafo
void adicionar_aresta(grafo *g, int v1, int v2, int peso);

//encontra o indice do caractere
int indice(grafo *g, char c);

//monta um grafo a partir de um arquivo
grafo * iniciar_grafo(grafo *g);

//imprime o grafo na forma lista de adjacencia
void imprime_grafo(grafo* g);


//heap

//cria a estrutura de um heap a partir do tamanho do grafo
heap * criar_heap(int capacidade);

//crai novo vertice para o grafo
no_heap * novo_no_heap(int v, int chave);

//realiza a troca entre dois nos do tipo no_heap
void trocar(no_heap ** a, no_heap ** b);

//heapfica em um determinado indice
void heapficar(heap *h, int indice);

//verifica se o heap esta vazio
int vazio(heap *h);

//retica o menor no do heap
no_heap * retira_menor_no_heap(heap *h);

//diminui a chave do vertice v
void diminuir_chave(heap *h, int v, int chave);

//verifica se v esta no heap ou nao
int existe(heap *h, int v);

//imprime a mst
void imprimir_mst(int *arr, int n, grafo *g);

//algoritmo de prim
void prim(grafo *g);

//retorna o peso de uma determinada aresta
int retorna_peso(grafo *g, int a, int b);

#endif