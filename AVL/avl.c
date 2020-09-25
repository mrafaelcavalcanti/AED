#include <stdio.h>
#include <stdlib.h>
#include "avl.h"



void inicializar(arvore *raiz){
  *raiz = NULL;
}



arvore adicionar(int valor, arvore raiz, int *cresceu){
  //caso base, a raiz eh vazia, cria um novo no e retorna ele
  if(raiz == NULL){
    arvore novo = (arvore) malloc(sizeof(struct no_avl));
    novo->dado = valor;
    novo->fb = 0;
    novo->dir = NULL;
    novo->esq = NULL;
    *cresceu = 1;
    return novo;
  }
  else{
    //o valor eh maior que o da raiz atual, insere a direita e atualiza os fatores de balanco
    if(valor > raiz->dado){
      raiz->dir = adicionar(valor, raiz->dir, cresceu);
      if(*cresceu){
        switch(raiz->fb){
          case -1:    //a raiz estava pesando a esquerda, apos inserir ela passa a ficar balanceada
            raiz->fb = 0;
            *cresceu = 0;
            break;
          case 0:   //a raiz ja estava balanceada, apos inserir ela passou a ficar pesada a direita
            raiz->fb = 1;
            *cresceu = 1;
            break;
          case 1:   //a raiz ja estava pesada a direita, apos inserir ela ficou ainda mais pesada
                    //o fator de balanco passa a ser 2 e ela precisa de uma rotacao para ajustar o balanco
            //raiz->fb = 1;//2
            *cresceu = 0;
            return rotacionar(raiz); //rotação a esquerda
        }
      }
      //o valor eh menor que o da atual raiz, insere a esquerda e atualiza os fatores de balanco
    }else if(valor < raiz->dado){
      raiz->esq = adicionar(valor, raiz->esq, cresceu);
      if(*cresceu){
        switch(raiz->fb){
          case 1:   //a raiz estava pesando a direita, apos inserir ela passa a ficar balanceada 
            raiz->fb = 0;
            *cresceu = 0;
            break;
          case 0:   //a raiz ja estava balanceada, apos inserir ela passa a ficar pesada a esquerda
            raiz->fb = -1;
            *cresceu = 1;
            break;
          case -1:    //a raiz ja esta pesada a esquerda, apos inserir ela ficou ainda mais pesada
                      //o fator de balanco passa a ser -2 e ela precisa de uma rotacao para ajusta o balanco
            //raiz->fb = -1;//-2
            *cresceu = 0;
            return rotacionar(raiz); //rotação a direita
        }
      }
    }
  }
  return raiz;
}



arvore remover(int valor, arvore raiz, int *diminuiu){
  //caso base, a raiz esta vazia
  if(raiz == NULL){
    *diminuiu = 0;
    return raiz;
  }else{
    //o valor a ser removido eh maior que o valor da raiz atual, remove a direita
    if(valor > raiz->dado){
      raiz->dir = remover(valor, raiz->dir, diminuiu);
      if(*diminuiu){
        switch(raiz->fb){
          case 1:   //a raiz estava pesando a direita, apos remover ela passou a ficar balanceada
            raiz->fb = 0;
            *diminuiu = 1;
            break;
          case 0:   //a raiz ja estava balanceada, apos remover ela ficou pesando a esquerda
            raiz->fb = -1;
            *diminuiu = 0;
            break;
          case -1:    //a raiz ja estava pesando a esquerda, apos remover ela ficou pesando ainda mais
                      //o fator de balanco passa a ser -2 e ela precisa de uma rotacao para ajustar o balanco
            if(raiz->esq->fb == 0){
              *diminuiu = 0;
            }
            //raiz->fb = -1;//-2
            return rotacionar(raiz); //rotação a direita
        }
      }
      //o valor a ser removido eh menor que o valor da raiz atual, remove a esquerda
    }else if(valor < raiz->dado){
      raiz->esq = remover(valor, raiz->esq, diminuiu);
      if(*diminuiu){
        switch(raiz->fb){
          case -1:    //a raiz estava pesando a esquerda, apos remover ela passa a ficar balanceada
            raiz->fb = 0;
            *diminuiu = 1;
            break;
          case 0:   //a raiz estava balanceada, apos remover ela passa a ficar pesada a direita
            raiz->fb = 1;
            *diminuiu = 0;
            break;
          case 1:   //a raiz ja estava pesando a direita, apos remover ela ficou pesando ainda mais
                    //o fator de balanco passa a ser 2 e ela precisa de uma rotacao para ajustar o balanco
            if(raiz->dir->fb == 0){
              *diminuiu = 0;
            }
            //raiz->fb = 1;//2
            return rotacionar(raiz); //rotação a esquerda
        }
      }
    }else{
      //o valor procurado foi achado
      if(raiz->dir != NULL && raiz->esq != NULL){   //verifica se os filhos nao sao nulos
        arvore aux;
        aux = raiz->dir;
        while(aux->esq!=NULL){    //procura o sucessor para substituir o valor a ser removido
          aux = aux->esq;
        }
        raiz->dado = aux->dado;
        raiz->dir = remover(aux->dado, raiz->dir, diminuiu);    //apos substituir, remove o sucessor
        if(*diminuiu){
        switch(raiz->fb){
          case 1:   //a raiz estava pesando a direita, apos remover ela passou a ficar balanceada
            raiz->fb = 0;
            *diminuiu = 1;
            break;
          case 0:   //a raiz ja estava balanceada, apos remover ela ficou pesando a esquerda
            raiz->fb = -1;
            *diminuiu = 0;
            break;
          case -1:     //a raiz ja estava pesando a esquerda, apos remover ela ficou pesando ainda mais
                       //o fator de balanco passa a ser -2 e ela precisa de uma rotacao para ajustar o balanco
            if(raiz->esq->fb == 0){
              *diminuiu = 0;
            }
            //raiz->fb = -1;//-2            
            return rotacionar(raiz); //rotação a direita
          }
        }
      }else{    //caso um de seus filhos seja nulo, substitui pelo filho nao nulo
        arvore aux_2 = raiz;
        if(raiz->dir != NULL){
          raiz = raiz->dir;
        }else if(raiz->esq != NULL){
          raiz = raiz->esq;
        }else{    //caso os dois filhos sejam nulos, a raiz passa a ser nula
          raiz = NULL;
        }
        free(aux_2);    //limpa da memoria a raiz antes da substituicao
        *diminuiu = 1;
        return raiz;
      }
    }
    return raiz;
  }
}


//rotacionar com ajuste de fatores de balanco
arvore rotacionar(arvore raiz){
  if(raiz->fb > 0){
    arvore p = raiz;
    arvore u = p->dir;

    if(u->fb == 1){
      u->fb = 0;
      p->fb = 0;
      p = rotacao_simples_esquerda(p);
    }else if(u->fb == 0){
      u->fb = -1;
      p->fb = 1;
      p = rotacao_simples_esquerda(p);
    }else{
      arvore v = p->dir->esq;
      switch(v->fb){
        case 1:
          p->fb = -1;
          u->fb = 0;
          break;
        case -1:
          p->fb = 0;
          u->fb = 1;
          break;
        case 0:
          p->fb = 0;
          u->fb = 0;
      }
      v->fb = 0;
      p = rotacao_dupla_esquerda(p);
    }
    return p;
  
  }else{
    arvore p = raiz;
    arvore u = p->esq;

    if(u->fb == -1){
      u->fb = 0;
      p->fb = 0;
      p = rotacao_simples_direita(p);
    }else if(u->fb == 0){
      u->fb = 1;
      p->fb = -1;
      p = rotacao_simples_direita(p);
    }else{
      arvore v = p->esq->dir;
      switch(v->fb){
        case 1:
          p->fb = 0;
          u->fb = -1;
          break;
        case -1:
          p->fb = 1;
          u->fb = 0;
          break;
        case 0:
          p->fb = 0;
          u->fb = 0;
      }
      v->fb = 0;
      p = rotacao_dupla_direita(p);
    }
    return p;
  }
  return raiz;
}



/*  rotacao simples a direita

    p             u
   / \           / \
  u   t3   =>   t1  p
 / \               / \
t1  t2            t2  t3

*/
arvore rotacao_simples_direita(arvore raiz){
  arvore p = raiz;
  arvore u = raiz->esq;
  p->esq = u->dir;
  u->dir = p;

  return u;
}



/*  rotacao simples a esquerda

    p               u
   / \             / \
  t1  u     =>    p   t3
     / \         / \
    t2  t3      t1  t2

*/
arvore rotacao_simples_esquerda(arvore raiz){
  arvore p = raiz;
  arvore u = raiz->dir;
  p->dir = u->esq;
  u->esq = p;

  return u;
}



/* rotacao dupla direita

    p              p                 v
   / \            / \              /   \
  u   t4   =>    v   t4    =>     u     p
 / \            / \              / \   / \
t1  v          u   t3           t1 t2 t3 t4
   / \        / \
  t2  t3     t1  t2

*/
arvore rotacao_dupla_direita(arvore raiz){
  raiz->esq = rotacao_simples_esquerda(raiz->esq);
  return rotacao_simples_direita(raiz);
}



/* rotacao dupla esquerda

    p               p                   v
   / \             / \                /   \
  t1  u     =>    t1  v       =>     p     u
     / \             / \            / \   / \
    v   t4          t2  u          t1 t2 t3 t4
   / \                 / \
  t2  t3              t3  t4

*/
arvore rotacao_dupla_esquerda(arvore raiz){
  raiz->dir = rotacao_simples_direita(raiz->dir);
  return rotacao_simples_esquerda(raiz);
}



void imprimir(arvore raiz) {
	printf("(");
	if(raiz != NULL) {
		imprimir_elemento(raiz);
		imprimir(raiz->esq);
		imprimir(raiz->dir);
	}
	printf(")");
}



void imprimir_elemento(arvore raiz) {
	printf("%d [%d]", raiz->dado, raiz->fb);
}



int altura(arvore raiz) {
	if(raiz == NULL) {
		return 0;
	}
	return 1 + maior(altura(raiz->dir), altura(raiz->esq));
}



int maior(int a, int b) {
	if(a > b)
		return a;
	else
		return b;
}



int maior_elemento(arvore raiz) {
	if(raiz == NULL)
			return -1;
	if(raiz->dir == NULL)
			return raiz->dado;
	else
			return maior_elemento(raiz->dir);
}



int menor_elemento(arvore raiz) {
	if(raiz == NULL)
			return -1;
	if(raiz->esq == NULL)
			return raiz->dado;
	else
			return maior_elemento(raiz->esq);
}



void pre_order(arvore raiz) {
	if(raiz != NULL) {
		imprimir_elemento(raiz);
		pre_order(raiz->esq);
		pre_order(raiz->dir);
	}
}



void pos_order(arvore raiz) {
	if(raiz != NULL) {
		pos_order(raiz->esq);
		pos_order(raiz->dir);
		imprimir_elemento(raiz);
	}
}



void in_order(arvore raiz) {
	if(raiz != NULL) {
		in_order(raiz->esq);
		imprimir_elemento(raiz);
		in_order(raiz->dir);
	}
}


//funcao para visualizacao da arvore
void display(arvore raiz, int nivel){
  int i;
  if(raiz == NULL){
    return;
  }else{
    display(raiz->dir, nivel+1);
    printf("\n");
    for(i=0; i<nivel; i++){
      printf("    ");
    }
    printf("%d", raiz->dado);
    display(raiz->esq, nivel+1);
  }
}
