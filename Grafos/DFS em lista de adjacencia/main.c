#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"


int main(void) {

    grafo *g;
    
    g = iniciar_grafo(g);
    
    imprime_grafo(g);
    
    DFS(g, 'B');
    
    return 0;    
}