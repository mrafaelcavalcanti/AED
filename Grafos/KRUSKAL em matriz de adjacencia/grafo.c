#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"




grafo * criar_grafo(int tamanho){
    //aloca um ponteiro para uma estrutura do tipo grafo
    grafo *g = malloc(sizeof(grafo));
    //o tamanho do grafo eh sua quantidade de vertices
	g->tamanho = tamanho;
    //armazena todas as letras dos vertices do grafo
	g->mapa = (char *) malloc(sizeof(char) * tamanho);
    //aloca memoriroa para uma matiz de ajacencia
	g->matrix = (int **) malloc(sizeof(int *) * tamanho);

    int i,j;
	for(i=0; i < tamanho; i++){
		g->matrix[i] = (int *) malloc(sizeof(int) * tamanho);        
    }

    //deixamos a matriz zerada, ela sera usada para indicar o peso de cada aresta
    //as arestas sao indicadas pelos indices [i] e [j]
	for(i=0; i < tamanho; i++) {
		for(j=0; j < tamanho; j++) {
            g->matrix[i][j] = 0;
            //printf("%d %d\n", i, j);
		}
	}
    return g;
}



//adicina a aresta na forma de um grafo nao direcionado
void adicionar_aresta(grafo *g, int v1, int v2, int peso){
    //adiciona a aresta de v1 para v2, onde [v1][v2] recebe seu peso
    g->matrix[v1][v2] = peso;
    //adiciona a aresta de v2 para v1, onde [v2][v1] recebe seu peso
    g->matrix[v2][v1] = peso;
}



//imprime o grafo na forma de matriz de adjacencia
void imprime_Adj_matrix(grafo *g){
    int i, j;

    printf("matriz adjacencia\n  ");

    for(i=0; i<g->tamanho;i++){
        printf("%c ", g->mapa[i]);
    }
    printf("\n");

    for(i=0; i<g->tamanho; i++) {
        printf("%c ", g->mapa[i]);
        for (j=0; j<g->tamanho; j++) {
            printf("%d ", g->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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

    int vertices, arestas, i, peso;
    char aresta1, aresta2, c;
    
    fscanf(input, "%d %d", &vertices, &arestas);    

    g = criar_grafo(vertices);

    for(i=0; i<vertices; i++){
        c = fgetc(input); //tira o \n do final de cada linha
        g->mapa[i] = fgetc(input);
        //printf("%c", g->mapa[i]);
    }

    for(i=0; i<arestas; i++){
        c = fgetc(input); //tira o \n do final de cada linha
        aresta1 = fgetc(input);
        aresta2 = fgetc(input);
        fscanf(input, " %d", &peso);
        adicionar_aresta(g, indice(g, aresta1), indice(g, aresta2), peso);
    }

    fclose(input);
    printf("passou\n");
    return g;
}


//algoritmo de kruskal

//ordena a lista de arestas em ordem crescente a partir de seu peso
void sort(lista_arestas *l){
    
    int i, j;
    aresta aux;

    for(i=1; i<l->quantidade; i++){
        for(j=0; j<l->quantidade-1; j++){
            if(l->arestas[j].peso > l->arestas[j+1].peso){
                aux = l->arestas[j];
                l->arestas[j] = l->arestas[j+1];
                l->arestas[j+1] = aux;
            }
        }
    }
}




int buscar(int pertence[], int v){
    return (pertence[v]);
}




void aplicar_uniao(grafo *g, int *pertence, int va, int vb){
    int i;

    for(i=0; i<g->tamanho; i++){
        if(pertence[i] == vb){
            pertence[i] = va;
        }
    }
}




void imprime(grafo *g, lista_arestas *mst){
    int custo, i;
    custo = 0;
    for(i=0; i<g->tamanho-1; i++){
        printf("%c - %c : %d\n", mst->arestas[i].u, mst->arestas[i].v, mst->arestas[i].peso);
        custo+=mst->arestas[i].peso;
    }
    printf("custo minimo: %d\n", custo);
}




void kruskal(grafo *g){

    //vetor auxiliar de tamanho igual ao grafo, cada indice indica um vertice do grafo
    int pertence[g->tamanho];

    int i, j, v1, v2, va, vb;

    aresta aux;

    //aloca um ponteiro para uma estrutura lista_arestas, lista
    lista_arestas *lista = (lista_arestas*)malloc(sizeof(lista_arestas));
    //aloca um ponteiro para um estrutura lista_arestas, mst (minimum spanning tree)
    lista_arestas *mst = (lista_arestas*)malloc(sizeof(lista_arestas));
    
    lista->quantidade = 0;
    mst->quantidade = 0;

    //copia todas as arestas para a lista
    for(i=1; i<g->tamanho; i++){
        for(j=0; j<i; j++){
            if(g->matrix[i][j] != 0){
                aux.peso = g->matrix[i][j];
                aux.u = g->mapa[i];
                aux.v = g->mapa[j];
                lista->arestas[lista->quantidade]=aux;
                lista->quantidade++;
            }
        }
    }
    

    //ordena a lista de arestas
    sort(lista);

    
    for(i=0; i<g->tamanho; i++){
        pertence[i] = i;
    }
    

    
    for(i=0; i<lista->quantidade; i++){
        //procura o indice da aresta no mapa do grafo
        v1 = indice(g, lista->arestas[i].u);
        v2 = indice(g, lista->arestas[i].v);

        //busca no vetor auxiliar onde esta v1 e v2
        va = buscar(pertence, v1);
        vb = buscar(pertence, v2);

        //se va e vb nao formorem um ciclo, inclui na mst
        if(va != vb){
            mst->arestas[mst->quantidade] = lista->arestas[i];
            mst->quantidade = mst->quantidade + 1;
            aplicar_uniao(g, pertence, va, vb);
        }
    }

    imprime(g, mst);
}
