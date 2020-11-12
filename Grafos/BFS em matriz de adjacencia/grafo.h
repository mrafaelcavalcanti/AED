#ifndef GRAFO_h
#define GRAFO_h

enum cor {branco, cinza, preto};


typedef struct fila{
    int tamanho_fila;
    int *itens;
    int frente;
    int tras;
}fila;

typedef struct grafo{
    int tamanho;
    char *mapa;
    int *visitados;
    int **matrix;
}grafo;

//fila

//cria a estrutura de uma fila a partir do tamanho do grafo
fila * cria_fila(int tamanho);

//adiciona um elemento a fila
void enfileirar(fila *f, int valor);

//retira um elemento da fila
int desenfileirar(fila *f);

//imprime a fila
void imprime_fila(fila *f, grafo *g);

//verifica se a fila esta vazia
int vazio(fila *f);

//grafo

//cria a estrutura de um grafo a partir do tamanho
grafo * criar_grafo(int tamanho);

//adiciona uma nova aresta ao grafo
void adicionar_aresta(grafo *g, int v1, int v2);

//imprime o grafo na forma matriz de adjacencia
void imprime_adj_matrix(grafo *g);

//encontra o indice do caractere
int indice(grafo *g, char c);

//monta um grafo a partir de um arquivo
grafo * iniciar_grafo(grafo *g);

//BFS

//auxiliar da BFS
void BFS(char v, fila *f, grafo *g);

//busa em largura
void busca_largura(int v, fila *f, grafo *g);



#endif