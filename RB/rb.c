#include <stdio.h>
#include <stdlib.h>
#include "rb.h"


arvore no_null;

//Inicializa a árvore vazia com a raiz = null e inicializa o nó nulo duplo preto que será utilizado no método remover

void inicializar(arvore *raiz) {
	*raiz = NULL;
	no_null = (arvore) malloc(sizeof(struct no_bst));
	no_null->cor = DUPLO_PRETO;
	no_null->dado = 0;
}



/*
adicionar um novo elemento a arvore
parametros:
    valor - elemento a ser adicionado
    raiz  - arvore onde o elemento sera adicionado (que eh um ponteiro de ponteiro)
*/

void adicionar(int valor, arvore *raiz){
    arvore posicao, pai, novo;
    posicao = *raiz;    //utiliza-se *raiz, por ser um ponteiro de ponteiro
    pai = NULL;

    /*navega na arvore ate achar a posicao adequada para o elemento
    eh preciso manter o ponteiro para o pai, pois apos encontrar a posicao vaga (NULL) 
    nao seria possivel acessar o pai com o ponteiro posicao->pai*/

    while(posicao != NULL){
        pai = posicao;
        if(valor > posicao->dado){
            posicao = posicao->dir;
        }else{
            posicao = posicao->esq;
        }
    }

    //apos encontar a posicao vaga, inicializa o novo elemento
    novo = (arvore) malloc(sizeof(struct no_bst));
    novo->dado = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = pai;
    novo->cor = VERMELHO;

    
    if(eh_raiz(novo)){    //atualiza a arvore caso esteja inserindo o primeiro elemento
        *raiz = novo;
    }else{      //se nao for a raiz, realiza a ligacao do pai com o novo elemento
        if(valor > pai->dado){
            pai->dir = novo;
        }else{
            pai->esq = novo;
        }
    }
    //apos inserir, verifica e ajusta a arvore
    ajustar(raiz, novo);
}



void ajustar(arvore *raiz, arvore elemento){
    //a arvore esta desajustada se o elemento e seu pais forem VERMELHOS
    while(cor(elemento->pai) == VERMELHO && cor(elemento) == VERMELHO){        
        if(cor(tio(elemento)) == VERMELHO){  //caso 1: cor do tio eh VERMELHO, desce o preto do avo
            tio(elemento)->cor = PRETO;
            elemento->pai->cor = PRETO;
            elemento->pai->pai->cor = VERMELHO;
            elemento = elemento->pai->pai;  //continua a verificacao a partir do avo, que agora eh VERMELHO
            //e pode ter gerado uma sequencia VERMELHO-VERMELHO
            continue;
        }
        
        //caso 2: rotacao simples
        if(eh_filho_esquerdo(elemento) && eh_filho_esquerdo(elemento->pai)){    //caso 2a: rotacao simples direita
            rotacao_simples_direita(raiz, elemento->pai->pai);
            elemento->pai->cor = PRETO;
            elemento->pai->dir->cor = VERMELHO;
            continue;
        }        
        
        if(!(eh_filho_esquerdo(elemento)) && !(eh_filho_esquerdo(elemento->pai))){   //caso 2b: rotacao simples esquerda
            rotacao_simples_esquerda(raiz, elemento->pai->pai);
            elemento->pai->cor = PRETO;
            elemento->pai->esq->cor = VERMELHO;
            continue;
        }

        //caso 3: rotacao dupla
        if(!(eh_filho_esquerdo(elemento)) && eh_filho_esquerdo(elemento->pai)){   //caso 3a: rotacao dupla direita
            elemento->pai->pai->cor = VERMELHO;
            elemento->cor = PRETO;
            rotacao_simples_esquerda(raiz, elemento->pai);
            rotacao_simples_direita(raiz, elemento->pai);            
            continue;
            
        }
        if(eh_filho_esquerdo(elemento) && !(eh_filho_esquerdo(elemento->pai))){   //caso 3b: rotacao dupla esquerda
            elemento->pai->pai->cor = VERMELHO;
            elemento->cor = PRETO;
            rotacao_simples_direita(raiz, elemento->pai);
            rotacao_simples_esquerda(raiz, elemento->pai);            
            continue;
        }
    }
    (*raiz)->cor = PRETO;   //apos as correcoes a raiz pode ter ficado VERMELHO, tornamos ela PRETO
}



/*
remove um elemento da arvore
parametros:
    valor - elemento a ser removido
    raiz  - arvore onde o elemento sera removido (que eh um ponteiro de ponteiro)
*/

void remover(int valor, arvore *raiz){
    arvore posicao;
    posicao = *raiz;

    while(posicao != NULL){
        if(valor == posicao->dado){
            if(posicao->esq == NULL && posicao->dir == NULL){   //elemento nao possui filhos
                if(eh_raiz(posicao)){   //remove raiz sem filhos
                    *raiz = NULL;
                    break;
                }
                //remove elemento que nao possui filhos e nao eh raiz
                if(posicao->cor == VERMELHO){   //se for VERMELHO, apenas remove e atualiza o ponteiro para o pai
                    if(eh_filho_esquerdo(posicao)){
                        posicao->pai->esq = NULL;
                    }else{
                        posicao->pai->dir = NULL;
                    }
                    break;
                }else{      //se for PRETO, substitui pelo DUPLO_PRETO e ajusta a arvore
                    no_null->pai = posicao->pai;
                    if(eh_filho_esquerdo(posicao)){
                        posicao->pai->esq = no_null;
                    }else{
                        posicao->pai->dir = no_null;
                    }
                    reajustar(raiz, no_null);
                    break;
                }
            }
            if(posicao->esq == NULL){   //elemento possui apenas um filho, o direito
                posicao->dir->cor = PRETO;
                if(eh_raiz(posicao)){
                    *raiz = posicao->dir;
                }else{
                    if(eh_filho_esquerdo(posicao)){
                        posicao->pai->esq = posicao->dir;
                    }else{
                        posicao->pai->dir = posicao->dir;
                    }
                }
                break;
            }
            if(posicao->dir == NULL){   //elemento possui apenas um filho, o esquerdo
                posicao->esq->cor = PRETO;
                if(eh_raiz(posicao)){
                    *raiz = posicao->esq;
                }else{
                    if(eh_filho_esquerdo(posicao)){
                        posicao->pai->esq = posicao->esq;
                    }else{
                        posicao->pai->dir = posicao->esq;
                    }
                }
                break;
            }
            //elemento possui dois filhos
            posicao->dado = maior_elemento(posicao->esq);
            remover(posicao->dado, &(posicao->esq));
        }
        if(valor > posicao->dado){      //elemento maior que o no atual
            posicao = posicao->dir;
        }else{                          //elemento menor que o no atual
            posicao = posicao->esq;
        }
    }
}



void reajustar(arvore *raiz, arvore elemento){

    /*Representacao das cores:
       X   -  VERMELHO
      (X)  -  PRETO
     ((X)) -  DUPLO_PRETO
      {X}  -  VERMELHO ou PRETO
    */


    /*CASO 1:
    - Existe o elemento [N] DUPLO_PRETO
    - [N] eh a raiz da arvore

    modifica a cor

        ((N))  =>  (N)

    Apos a modificacao:
    - [N] se torna PRETO
    */

    if(eh_raiz(elemento)){      //elemento eh a propria raiz
        elemento->cor = PRETO;
        return;
    }
    

    /*CASO 2:
    - Existe o elemento [N] DUPLO PRETO
    - O pai [P] eh PRETO
    - O irmao [S] eh VERMELHO
    - Os filhos [X][Y] do irmao [S] sao PRETO
    
    realiza uma rotacao simples (esquerda ou direita)

           (P)                 (S)
           / \                 / \
        ((N)) S      =>       P  (Y)
             / \             / \
           (X) (Y)       ((N)) (X)

    Apos a rotacao:
    - [P] se torna VERMELHO
    - [S] se torna PRETO
    - [X][Y] continuam PRETO
    - [N] continua DUPLO_PRETO
    */

    if(cor(elemento->pai) == PRETO && 
    cor(irmao(elemento)) == VERMELHO && 
    cor(irmao(elemento)->dir) == PRETO && 
    cor(irmao(elemento)->esq) == PRETO){    
        //verifica se eh o caso 2 esquerdo ou direito
        irmao(elemento)->cor = PRETO;
        elemento->pai->cor = VERMELHO;
        if(eh_filho_esquerdo(elemento)){
            rotacao_simples_esquerda(raiz, elemento->pai);
        }else{
            rotacao_simples_direita(raiz, elemento->pai);
        }
        
        //o caso 2 nao remove o DUPLO_PRETO
        reajustar(raiz, elemento);
        return;
    }
    

    /*CASO 3:
    - Existe o elemento [N] DUPLO_PRETO
    - Seu pai [P] eh PRETO
    - Seu irmao [S] eh PRETO
    - Os filhos [X][Y] do irmao [S] sao PRETO

    Sobe o duplo preto

           (P)              ((P))
           / \               / \
       ((N)) (S)      =>   (N)  S
             / \               / \
           (X) (Y)           (X) (Y)

    Apos a modificacao:
    - [P] se torna DUPLO_PRETO
    - [S] se torna VERMELHO
    - [X][Y] continuam PRETO
    - [N] se torna PRETO    
    */

    if(cor(elemento->pai) == PRETO && 
    cor(irmao(elemento)) == PRETO && 
    cor(irmao(elemento)->dir) == PRETO && 
    cor(irmao(elemento)->esq) == PRETO){

        irmao(elemento)->cor = VERMELHO;
        elemento->pai->cor = DUPLO_PRETO;
        arvore aux;
        aux = elemento->pai;
        retira_duplo_preto(raiz, elemento);

        //o caso 3 nao remove o DUPLO_PRETO
        reajustar(raiz, aux);
        return;
    }


    /*CASO 4:
    - Existe o elemento [N] DUPLO_PRETO
    - O pai [P] eh VERMELHO
    - O irmao [S] eh PRETO
    - Os filhos [X][Y] do irmao [S] sao PRETO

    modifica as cores

            P                (P)
           / \               / \
       ((N)) (S)     =>    (N)  S
             / \               / \
           (X) (Y)           (X) (Y)
    
    Apos a modificacao:
    - [P] se torna PRETO
    - [S] se torna VERMELHO
    - [X][Y] continuam PRETO
    - [N] se torna PRETO
    */

    if(cor(elemento->pai) == VERMELHO &&
    cor(irmao(elemento)) == PRETO &&
    cor(irmao(elemento)->dir) == PRETO &&
    cor(irmao(elemento)->esq) == PRETO){
        
        if(irmao(elemento)!=NULL){
            irmao(elemento)->cor = VERMELHO;
        }
        elemento->pai->cor = PRETO;

        retira_duplo_preto(raiz, elemento);

        return;
    }


    //caso 5 e 6 ficao mais faceis separando o esquerdo do direito
    
    /*CASO 5a:
    - Existe o elemento [N] DUPLO_PRETO
    - O pai [P] pode ser VERMELHO ou PRETO
    - O irmao [S] eh PRETO
    - O filho [X] de [S] eh VERMELHO
    - O filho [Y] de [S] eh PRETO

    rotacao simples direita em [S]
    
           {P}                 {P}
           / \                 / \
       ((N)) (S)      =>   ((N)) (X)
             / \                   \
            X  (Y)                  S
                                     \
                                     (Y)
    
    Apos a rotacao:
    - [P] nao muda de cor
    - [S] se torna VERMELHO
    - [X] se torna PRETO
    - [Y] continua PRETO
    - [N] contina DUPLO_PRETO
    */

    if(cor(irmao(elemento)) == PRETO && 
    cor(irmao(elemento)->dir) == PRETO && 
    cor(irmao(elemento)->esq) == VERMELHO &&
    eh_filho_esquerdo(elemento) == 1){
        
        irmao(elemento)->cor = VERMELHO;
        irmao(elemento)->esq->cor = PRETO;
        
        rotacao_simples_direita(raiz, elemento->pai->dir);

        //o caso 5a nao remove o DUPLO_PRETO
        reajustar(raiz, elemento);
        return;
    }


    /*CASO 5b:
    - Existe o elemento [N] DUPLO_PRETO
    - O pai [P] pode ser VERMELHO ou PRETO
    - O irmao [S] eh PRETO
    - O filho [X] de [S] eh PRETO
    - O filho [Y] de [S] eh VERMELHO

    rotacao simples esquerda em [S]
    
           {P}                 {P}
           / \                 / \
         (S) ((N))    =>     (Y) ((N))
         / \                 /
       (X)  Y               S
                           /
                         (X)
    
    Apos a rotacao:
    - [P] nao muda de cor
    - [S] se torna VERMELHO
    - [X] continua PRETO
    - [Y] se torna PRETO
    - [N] contina DUPLO_PRETO
    */

    if(cor(irmao(elemento)) == PRETO && 
    cor(irmao(elemento)->dir) == VERMELHO && 
    cor(irmao(elemento)->esq) == PRETO &&
    eh_filho_esquerdo(elemento) == 0){
        
        irmao(elemento)->cor = VERMELHO;
        irmao(elemento)->dir->cor = PRETO;
        
        rotacao_simples_esquerda(raiz, elemento->pai->esq);

        //o caso 5b nao remove o DUPLO_PRETO
        reajustar(raiz, elemento);
        return;
    }

    
    /*CASO 6a:
    - Existe o elemento [N] DUPLO_PRETO
    - O pai [P] pode ser VERMELHO ou PRETO
    - O irmao [S] eh PRETO
    - O filho [X] de [S] pode ser VERMELHO ou PRETO
    - O filho [Y] de [S] eh VERMELHO

    rotacao simples esquerda

           {P}                  {S}
           / \                  / \
       ((N)) (S)      =>      (P)  (Y)
             / \              / \
           {X}  Y           (N) {X}
    
    Apos a rotacao:
    - [P] se torna PRETO
    - [S] se torna a antiga cor de [P]
    - [X] nao muda de cor
    - [Y] se torna PRETO
    - [N] se torna PRETO
    */

    if(cor(irmao(elemento)) == PRETO &&
    cor(irmao(elemento)->dir) == VERMELHO &&
    eh_filho_esquerdo(elemento) == 1){
        
        irmao(elemento)->cor = elemento->pai->cor;
        irmao(elemento)->dir->cor = PRETO;
        elemento->pai->cor = PRETO;
        arvore aux;
        aux = elemento->pai;
        retira_duplo_preto(raiz, elemento);

        rotacao_simples_esquerda(raiz, aux);
        return;
    }


    /*CASO 6b:
    - Existe o elemento [N] DUPLO_PRETO
    - O pai [P] pode ser VERMELHO ou PRETO
    - O irmao [S] eh PRETO
    - O filho [X] de [S] eh VERMELHO
    - O filho [Y] de [S] pode ser VERMELHO ou PRETO

    rotacao simples direita

           {P}               {S}
           / \               / \
         (S) ((N))    =>   (X) (P)
         / \                   / \
        X  {Y}               {Y} (N)
    
    Apos a rotacao:
    - [P] se torna PRETO
    - [S] se torna a antiga cor de [P]
    - [X] se torna PRETO
    - [Y] nao muda de cor
    - [N] se torna PRETO
    */

    if(cor(irmao(elemento)) == PRETO &&
    cor(irmao(elemento)->esq) == VERMELHO &&
    eh_filho_esquerdo(elemento) == 0){
        
        irmao(elemento)->cor = elemento->pai->cor;
        irmao(elemento)->esq->cor = PRETO;
        elemento->pai->cor = PRETO;

        arvore aux;
        aux = elemento->pai;
        retira_duplo_preto(raiz, elemento);

        rotacao_simples_direita(raiz, aux);        
        return;
    }
}



void retira_duplo_preto(arvore *raiz, arvore elemento){
    if(elemento == no_null){
        if(eh_filho_esquerdo(elemento)){
            elemento->pai->esq = NULL;
        }else{
            elemento->pai->dir = NULL;
        }
    }else{
        elemento->cor = PRETO;
    }
}



/*Rotação Simples a Direita
Antes da rotacao: 
cor(p) = Preto
cor(u) = cor(v) = Vermelho
       p             u
      / \           / \
     u  t2    =>   v   p
    / \               / \
   v  t1             t1 t2
Apos a rotacao: 
cor(u) = Preto
cor(v) = cor(p) = Vermelho
*/

void rotacao_simples_direita(arvore *raiz, arvore pivo){
    arvore u, t1;
    u = pivo->esq;
    t1 = u->dir;
    //para fazer a ligacao da raiz da sub-arvore resultante com seu pai, eh preciso saber se o pivo era um filho esquerdo ou direito
    int posicao_pivo_esq = eh_filho_esquerdo(pivo);
    pivo->esq = t1;
    
    if(t1 != NULL){
        t1->pai = pivo;
    }

    u->dir = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;

    
    if(eh_raiz(u)){
        *raiz = u;
    }else{
        if(posicao_pivo_esq){
            u->pai->esq = u;
        }else{
            u->pai->dir = u;
        }
    }
    
}


/*Rotação Simples a Direita
Antes da rotacao: 
cor(p) = Preto
cor(u) = cor(v) = Vermelho

    p             u
   / \           / \
  t2  u    =>   p   v
     / \       / \
    t1  v     t2 t1

Apos a rotacao: 
cor(u) = Preto
cor(v) = cor(p) = Vermelho
*/

void rotacao_simples_esquerda(arvore *raiz, arvore pivo){
    arvore u, t1;
    u = pivo->dir;
    t1 = u->esq;
    //para fazer a ligacao da raiz da sub-arvore resultante com seu pai, eh preciso saber se o pivo era um filho esquerdo ou direito
    int posicao_pivo_esquerdo = eh_filho_esquerdo(pivo);
    pivo->dir = t1;

    if(t1 != NULL){
        t1->pai = pivo;
    }

    u->esq = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;


    if(eh_raiz(u)){
        *raiz = u;
    }else{
        if(posicao_pivo_esquerdo){
            u->pai->esq = u;
        }else{
            u->pai->dir = u;
        }
    }

}



enum cor cor(arvore elemento){
    enum cor c;
    if(elemento==NULL || elemento->cor == PRETO){   //por definicao, o NULL eh PRETO
        c = PRETO;
    }else{
        c = VERMELHO;
    }
    return c;
}



int eh_raiz(arvore elemento){
    return (elemento->pai == NULL);
}



int eh_filho_esquerdo(arvore elemento){
    return (elemento->pai != NULL && elemento == elemento->pai->esq);
}



arvore tio(arvore elemento){
    return irmao(elemento->pai);
}



arvore irmao(arvore elemento){
    if(eh_filho_esquerdo(elemento)){
        return elemento->pai->dir;
    }else{
        return elemento->pai->esq;
    }
}



void imprimir(arvore raiz){
	printf("(");
	if(raiz != NULL){
		imprimir_elemento(raiz);
		imprimir(raiz->esq);
		imprimir(raiz->dir);
	}
	printf(")");
}



int altura(arvore raiz){
	if(raiz == NULL){
		return 0;
	}
	return 1 + maior(altura(raiz->dir), altura(raiz->esq));
}



int maior(int a, int b){
	if(a > b){
		return a;
    }else{
		return b;
    }
}



int maior_elemento(arvore raiz){
	if(raiz == NULL){
		return -1;
    }if(raiz->dir == NULL){
		return raiz->dado;
    }else{
		return maior_elemento(raiz->dir);
    }
}



int menor_elemento(arvore raiz){
	if(raiz == NULL){
		return -1;
    }if(raiz->esq == NULL){
		return raiz->dado;
    }else{
		return maior_elemento(raiz->esq);
    }
}



void pre_order(arvore raiz){
	if(raiz != NULL){
		imprimir_elemento(raiz);
		pre_order(raiz->esq);
		pre_order(raiz->dir);
	}
}



void pos_order(arvore raiz){
	if(raiz != NULL){
		pos_order(raiz->esq);
		pos_order(raiz->dir);
		imprimir_elemento(raiz);
	}
}



void in_order(arvore raiz){
	if(raiz != NULL){
		in_order(raiz->esq);
		imprimir_elemento(raiz);
		in_order(raiz->dir);
	}
}



void imprimir_elemento(arvore raiz){
	switch(raiz->cor){
		case PRETO:
			printf("\x1b[30m[%d]\x1b[0m", raiz->dado);
			break;
		case VERMELHO:
			printf("\x1b[31m[%d]\x1b[0m", raiz->dado);
			break;
		case DUPLO_PRETO:
			printf("\x1b[32m[%d]\x1b[0m", raiz->dado);
			break;
	}
}



void imprime_arvore(arvore raiz){
    printf("\n\n");
    printf("\x1b[96m------------------------------\x1b[0m\n");
    display(raiz, 0);
    printf("\n\n");
    printf("\x1b[96m------------------------------\x1b[0m\n");

    //possiveis outras cores para o terminal
    //printf("\x1b[31m------------------------------\x1b[0m\n");
    //printf("\x1b[32m------------------------------\x1b[0m\n");
    //printf("\x1b[33m------------------------------\x1b[0m\n");
    //printf("\x1b[34m------------------------------\x1b[0m\n");
    //printf("\x1b[35m------------------------------\x1b[0m\n");
    //printf("\x1b[36m------------------------------\x1b[0m\n");
    //printf("\x1b[37m------------------------------\x1b[0m\n");
    //printf("\x1b[40m------------------------------\x1b[0m\n");
    //printf("\x1b[41m------------------------------\x1b[0m\n");
    //printf("\x1b[42m------------------------------\x1b[0m\n");
    //printf("\x1b[43m------------------------------\x1b[0m\n");
    //printf("\x1b[44m------------------------------\x1b[0m\n");
    //printf("\x1b[45m------------------------------\x1b[0m\n");
    //printf("\x1b[46m------------------------------\x1b[0m\n");
    //printf("\x1b[47m------------------------------\x1b[0m\n");
    //printf("\x1b[90m------------------------------\x1b[0m\n");
    //printf("\x1b[91m------------------------------\x1b[0m\n");
    //printf("\x1b[92m------------------------------\x1b[0m\n");
    //printf("\x1b[93m------------------------------\x1b[0m\n");
    //printf("\x1b[94m------------------------------\x1b[0m\n");
    //printf("\x1b[95m------------------------------\x1b[0m\n");
    //printf("\x1b[96m------------------------------\x1b[0m\n");
    //printf("\x1b[97m------------------------------\x1b[0m\n");
    //printf("\x1b[100m------------------------------\x1b[0m\n");
    //printf("\x1b[101m------------------------------\x1b[0m\n");
    //printf("\x1b[102m------------------------------\x1b[0m\n");
    //printf("\x1b[103m------------------------------\x1b[0m\n");
    //printf("\x1b[104m------------------------------\x1b[0m\n");
    //printf("\x1b[105m------------------------------\x1b[0m\n");
    //printf("\x1b[106m------------------------------\x1b[0m\n");
    //printf("\x1b[107m------------------------------\x1b[0m\n");
}



void display(arvore raiz, int nivel){
    int i;
    if(raiz == NULL){
        return;
    }else{
        display(raiz->dir, nivel+1);
        printf("\n");
        for(i=0; i<nivel; i++){
            printf("      ");
        }
        printf("%d ", nivel);
        switch(raiz->cor){
		    case PRETO:
			    printf("\x1b[40m\x1b[1m[%d]\x1b[0m\x1b[0m", raiz->dado);
			    break;
		    case VERMELHO:
			    printf("\x1b[41m\x1b[1m[%d]\x1b[0m\x1b[0m", raiz->dado);
			    break;
		    case DUPLO_PRETO:
    			printf("\x1b[32m[%d]\x1b[0m", raiz->dado);
	    		break;
	    }
        display(raiz->esq, nivel+1);
    }
}