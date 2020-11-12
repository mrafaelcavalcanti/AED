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
    
    //g->visitado = malloc(tamanho * sizeof(no_vertice));

    int i;
    for(i=0; i<tamanho; i++){
        //inicia os ponteiros para as listas com NULL
        g->lista_adjacencia[i] = NULL;
    }
    return g;
}




no_vertice * novo_vertice(int v, int peso){
    no_vertice * novo = malloc(sizeof(no_vertice));
    novo->vertice = v;
    novo->peso = peso;
    novo->prox = NULL;
    return novo;
}



//adicina a aresta na forma de um grafo nao direcionado
void adicionar_aresta(grafo *g, int v1, int v2, int peso){
    //adiciona a aresta de v1 para v2
    no_vertice * novo = novo_vertice(v2, peso);
    novo->prox = g->lista_adjacencia[v1]; //[v1[0]-'A'] em caso de se usar caracteres ao inves de inteiros
    g->lista_adjacencia[v1] = novo;

    //adiciona a aresta de v2 para v1
    novo = novo_vertice(v1, peso);
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
        //printf("%c\n", g->mapa[i]);
    }

    for(i=0; i<arestas; i++){
        c = fgetc(input); //tira o \n do final de cada linha
        aresta1 = fgetc(input);
        aresta2 = fgetc(input);
        fscanf(input, " %d", &peso);
        adicionar_aresta(g, indice(g, aresta1), indice(g, aresta2), peso);
    }

    fclose(input);
    return g;

}



//heap
no_heap * novo_no_heap(int v, int chave){
    no_heap * novo = (no_heap*)malloc(sizeof(no_heap));
    novo->v = v;
    novo->chave = chave;
    return novo;
}



//cria um estrutura de heap
heap * criar_heap(int capacidade){
    heap *novo = (heap *)malloc(sizeof(heap));
    novo->posicao = (int*)malloc(capacidade*sizeof(int));
    novo->tamanho_heap = 0;
    novo->capacidade = capacidade;
    novo->matrix = (no_heap**)malloc(capacidade*sizeof(no_heap));
    return novo;
}



//heapfica em um determinado indice
void heapficar(heap *h, int indice){
    
    int menor, esq, dir;

    menor = indice;
    esq = 2*indice+1;
    dir = 2*indice+2;

    //encontra o menor
    if(esq < h->tamanho_heap && h->matrix[esq]->chave < h->matrix[menor]->chave){
        menor = esq;
    }

    if(dir < h->tamanho_heap && h->matrix[dir]->chave < h->matrix[menor]->chave){
        menor = dir;
    }

    if(menor != indice){
        //nos que serao trocados
        no_heap *menor_no = h->matrix[menor];
        no_heap *indice_no = h->matrix[indice];

        //troca as posicoes
        h->posicao[menor_no->v] = indice;
        h->posicao[indice_no->v] = menor;

        //troca os nos
        trocar(&h->matrix[menor], &h->matrix[indice]);
        heapficar(h, menor);
    }
}



//verifica se o heap esta vazio
int vazio(heap *h){
    return h->tamanho_heap == 0;
}



//retica o menor no do heap
no_heap * retira_menor_no_heap(heap *h){
    
    //verifica se o heap h esta vazio, se etiver retorna NULL
    if(vazio(h)){
        return NULL;
    }

    //raiz armazena o no raiz
    no_heap *raiz = h->matrix[0];

    //substitui o no raiz pelo ultimo no
    no_heap *ultimo = h->matrix[h->tamanho_heap-1];    
    h->matrix[0] = ultimo;

    //atualiza a posicao do ultimo no
    h->posicao[raiz->v] = h->tamanho_heap-1;
    h->posicao[ultimo->v] = 0;

    //reduz o tamanho do heap
    h->tamanho_heap--;
    
    //heapfica a raiz
    heapficar(h, 0);

    return raiz;
}



//diminui a chave do vertice v
void diminuir_chave(heap *h, int v, int chave){
    
    //indice recebe o indice de v no heap
    int indice = h->posicao[v];

    //atualiza a chave do no indice
    h->matrix[indice]->chave = chave;
    
    while(indice && h->matrix[indice]->chave < h->matrix[(indice - 1)/2]->chave){
        //troca o no pelo seu pai        
        h->posicao[h->matrix[indice]->v] = (indice - 1)/2;
        h->posicao[h->matrix[(indice - 1)/2]->v] = indice;
        trocar(&h->matrix[indice], &h->matrix[(indice - 1)/2]);

        //atualiza o indice
        indice = (indice - 1)/2;
    }
}



//verifica se v esta no heap ou nao
int existe(heap *h, int v){
    if(h->posicao[v] < h->tamanho_heap){
        return 1;
    }else{
        return 0;
    }
}



//realiza a troca entre dois nos do tipo no_heap
void trocar(no_heap ** a, no_heap ** b){
    no_heap* aux = *a;
    *a = *b;
    *b = aux;
}




void imprimir_mst(int *arr, int n, grafo *g){
    int i, peso, custo = 0;
    for(i=1; i<n; i++){
        peso = retorna_peso(g, arr[i],i);
        printf("%c - %c : %d\n", g->mapa[arr[i]], g->mapa[i], peso);
        custo +=peso;
    }
    printf("custo minimo: %d\n", custo);
}




void prim(grafo *g){

    //v recebe o o tamanho de g (sua quantidade de vertices)
    int v = g->tamanho;
    //vetor onde sera armazenado a mst
    int pai[v];
    //vetor de chaves, sera usado para escolhar a aresta de menor peso
    int chave[v];

    //aloca um ponteiro para um heap de tamanho v
    heap *h = criar_heap(v);

    //inicializa o heap com todos os vertices, exceto o primeiro vertice, com um valor infinito
    //infinito eh um valor abstrato que nao pode ser representado, 9999999 ira representar o infinito
    int i;
    for(i=1; i<v; i++){
        pai[i]=-1;
        chave[i] = 9999999;
        h->matrix[i] = novo_no_heap(i, chave[i]);
        h->posicao[i] = i;
    }


    //o chave do peimeiro vertice eh inicializado como 0 para que ele seja removido primeiro
    chave[0]=0;
    h->matrix[0] = novo_no_heap(0, chave[0]);
    h->posicao[0]=0;

    h->tamanho_heap = v;

    //heap tem todos os nos ainda nao adicionados a mst
    while(!vazio(h)){
        //novo retira o vertice de menor valor do heap
        no_heap* novo = retira_menor_no_heap(h);
        //u rebece o numero do vertice de menor valor
        int u = novo->v;

        //percorre todos os vertices adjacentes a u e atualiza suas chaves
        no_vertice *p = g->lista_adjacencia[u];
        while(p!=NULL){
            int i = p->vertice;

            //se i ainda nao esta na mst e o peso de (u-i) for menor que a chave de i
            //atualiza a chave e o pai de i
            if(existe(h, i) && p->peso < chave[i]){
                chave[i]=p->peso;
                pai[i]=u;
                diminuir_chave(h, i, chave[i]);
            }
            p = p->prox;
        }
    }

    imprimir_mst(pai, v, g);
}




int retorna_peso(grafo *g, int a, int b){
    no_vertice *aux = g->lista_adjacencia[a];
    int peso =-1;
    while(aux!=NULL){
        if(aux->vertice == b){
            peso = aux->peso;
            break;
        }else{
            aux = aux->prox;
        }
    }
    return peso;
}
