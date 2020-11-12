#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"


int main(void) {
    
    grafo *g;
    fila *f;

    g = iniciar_grafo(g);
    f = cria_fila(g->tamanho);
    
    imprime_adj_matrix(g);
        
    BFS('F', f, g);
    
    return 0;
}
