#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"


grafo * criar_grafo(int tamanho){
    //aloca um ponteiro para uma estrutura do tipo grafo
    grafo * g = malloc(sizeof(grafo));
    //o tamanho do grafo eh sua quantidade de vertices
    g->tamanho = tamanho;
    //armazena todas as letras dos vertices do grafo
    g->mapa = (char *)malloc(sizeof(char) * tamanho);
    //cada indice do vetor contem um ponteiro para uma lista encadeada
    g->lista_adjacencia = malloc(tamanho * sizeof(no_vertice));
    //visitados sera usado na DFS, indica por quais vertices o algoritmo ja passou
    g->visitado = malloc(tamanho * sizeof(no_vertice));

    int i;
    for(i=0; i<tamanho; i++){
        //inicia os ponteiros para as listas com NULL
        g->lista_adjacencia[i] = NULL; 
        //inicia os vertices por onde o algoritmo de busca vai passar com 0
        g->visitado[i] = 0; 
    }
    return g;
}




no_vertice * novo_vertice(int v){
    no_vertice * novo = malloc(sizeof(no_vertice));
    novo->vertice = v;
    novo->prox = NULL;
    return novo;
}



//adicina a aresta na forma de um grafo nao direcionado
void adicionar_aresta(grafo *g, int v1, int v2){
    //adiciona a aresta de v1 para v2
    no_vertice * novo = novo_vertice(v2);
    novo->prox = g->lista_adjacencia[v1]; //[v1[0]-'A'] em caso de se usar caracteres ao inves de inteiros
    g->lista_adjacencia[v1] = novo;

    //adiciona a aresta de v2 para v1
    novo = novo_vertice(v1);
    novo->prox = g->lista_adjacencia[v2];
    g->lista_adjacencia[v2] = novo;
}



//imprime o grafo na forma de lista de adjacencia
void imprime_grafo(grafo* g){
    int i;
    for (i = 0; i < g->tamanho; i++){
        no_vertice* p = g->lista_adjacencia[i];
        printf("%c", g->mapa[i]);
        while (p!=NULL){
            printf("-> %c", g->mapa[p->vertice]);
            p = p->prox;
        }
        printf("\n");
    }
    printf("\n");
}



//auxiliar da busca em profundidade
void DFS(grafo *g, char v){
    int vertice = indice(g, v);
    if(vertice==-1){
        printf("este vertice nao existe\n");
    }else{
        printf("DFS a partir de: %c\n", g->mapa[vertice]);
        busca_profundidade(g, vertice);
    }
}




void busca_profundidade(grafo *g, int v){
    no_vertice * lista_adjacencia = g->lista_adjacencia[v];
    no_vertice * temp = lista_adjacencia;
    
    //o no atual eh marcado como visitado(1) e imprime ele
    g->visitado[v] = 1; //[v[0]-'A'] em caso de se usar caracteres ao inves de inteiros
    printf("visitado: %c\n", g->mapa[v]);

    //percorre todos os vertices adjacentes ao vertice atual
    while(temp!=NULL){
        int vertice_conectado = temp->vertice;
        if(g->visitado[vertice_conectado] == 0){
            busca_profundidade(g, vertice_conectado);
        }
        temp = temp->prox;
    }
}



//retorna o indice do vertice no mapa, caso o vertice nao exista retorna -1
int indice(grafo *g, char c){
    int i;
    for(i=0; i<g->tamanho; i++){
        if(c == g->mapa[i]){
            return i;
        }
    }
    return -1;
}



//monta o grafo a partir de um arquivo
grafo * iniciar_grafo(grafo *g){
    FILE *input;
    input = fopen("entrada.txt", "r");
    
    int vertices, arestas, i;
    char aresta1, aresta2, c;

    fscanf(input, "%d %d", &vertices, &arestas);
    g = criar_grafo(vertices);

    for(i=0; i<vertices; i++){
        c = fgetc(input); //tira o \n do final de cada linha
        g->mapa[i] = fgetc(input);
        //printf("%c\n", g->mapa[i]);
    }

    for(i=0; i<arestas; i++){
        c = fgetc(input); //tira o \n do final de cada linha
        aresta1 = fgetc(input);
        aresta2 = fgetc(input);
        adicionar_aresta(g, indice(g, aresta1), indice(g, aresta2));
    }

    fclose(input);
    return g;
}


