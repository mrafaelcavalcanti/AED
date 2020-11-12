#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"


//verifica se a fila esta vazia, se estiver retorna 1, senao retorna 0
int vazio(fila *f){
    if(f->frente==-1 || f->frente > f->tras){
        return 1;
    }else{
        return 0;
    }
}




fila * cria_fila(int tamanho){
    fila *f = malloc(sizeof(fila));
    f->itens = malloc(sizeof(int)*(tamanho*10));
    f->frente = -1;
    f->tras = -1; 
    f->tamanho_fila = tamanho*10;
    /*tamanho*10 para evitar um possivel overflow da fila, nao 
    precisa ser nescessariamente um produto, somar +1 
    poderia evitaria o overflow da mesma maneira*/
    
    for(int i=0; i<f->tamanho_fila; i++){
        f->itens[i] = 0;
    }
    return f;
}




void enfileirar(fila *f, int valor){
    if(f->tras == f->tamanho_fila-1){
        printf("overflow da fila\n");
    }else{
        if(f->frente == -1){
            f->frente = 0;
        }
        f->tras++;
        f->itens[f->tras] = valor;
    }
}




int desenfileirar(fila *f){
    int item_deletado;

    if(vazio(f)){
        printf("underflow da fila\n");
        exit(1);
    }

    item_deletado = f->itens[f->frente];
    f->frente++;

    return item_deletado;
}




void imprime_fila(fila *f, grafo *g){
    int i = f->frente;

    if(vazio(f)){
        printf("fila vazia\n");
    }else{
        printf("fila: ");
        for(i=f->frente; i<f->tras+1; i++){
            printf("%c ", g->mapa[f->itens[i]]);
        }
        printf("\n");
    }
}




grafo * criar_grafo(int tamanho){
    //aloca um ponteiro para uma estrutura do tipo grafo
    grafo *g = malloc(sizeof(grafo));
    //o tamanho do grafo eh sua quantidade de vertices
	g->tamanho = tamanho;
    //armazena todas as letras dos vertices do grafo
	g->mapa = (char *) malloc(sizeof(char) * tamanho);
    //aloca memoriroa para uma matiz de ajacencia
	g->matrix = (int **) malloc(sizeof(int *) * tamanho);
    //visitados sera usado na BFS, indica por quais vertices o algoritmo ja passou
    g->visitados = malloc(sizeof(int)*(tamanho+1));

    int i,j;
	for(i=0; i < tamanho; i++){
		g->matrix[i] = (int *) malloc(sizeof(int) * tamanho);
        //inicia os vertices por onde o algoritmo de busca vai passar com branco
        g->visitados[i] = branco;
    }
    //como alocamos tamanho+1 para evitar um possivel estouro do array e no  laco anterior 
    //percorremos g->visitados ate n, iremos alocar a ultima posicao
    g->visitados[tamanho] = branco;

    //percorre-se a matriz muda para 1 quando i==j para criar uma aresta do vertice para ele mesmo
	for(i =0; i < tamanho; i++) {
		for(j=0; j < tamanho; j++) {
			if(i==j) {
				g->matrix[i][j] = 1;
			} else {
				g->matrix[i][j] = 0;
			}
		}
	}
    return g;
}



//adicina a aresta na forma de um grafo nao direcionado
void adicionar_aresta(grafo *g, int v1, int v2){
    //adiciona a aresta de v1 para v2
    g->matrix[v1][v2] = 1;
    //adiciona a aresta de v2 para v1
    g->matrix[v2][v1] = 1;
}



//imprime o grafo na forma de matriz de adjacencia
void imprime_adj_matrix(grafo *g){
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



//auxiliar da busca em largura
void BFS(char v, fila *f, grafo *g){
    int vertice = indice(g, v);
    if(vertice==-1){
        printf("este vertice nao existe\n");
    }else{
        busca_largura(vertice, f, g);
    }
}




void busca_largura(int v, fila *f, grafo *g){
    
    //insere na fila o vertice que vai ser visitado
    enfileirar(f,v);
    //o vertice vai ser visitado, marca como cinza
    g->visitados[v] = cinza;

    printf("BFS a partir de: %c\n", g->mapa[v]);
    
    int i;
    imprime_fila(f, g);//apenas para ver o funcionamaneto da fila
    while(!vazio(f)){        
        //o vertice foi visitado, remove da fila
        v = desenfileirar(f);
        
        //o vertice foi visitado, marca como preto
        g->visitados[v] = preto;
        printf("visitado: %c\n", g->mapa[v]);

        for(i=0; i<g->tamanho; i++){
            if(g->matrix[v][i]==1 && g->visitados[i]==branco){
                enfileirar(f, i); //insere na fila o vertice que vai ser visitado
                g->visitados[i] = cinza; //o vertice vai ser visitado, marca como cinza
            }
        }
        imprime_fila(f, g);//apenas para ver o funcionamaneto da fila
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
