#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

no* inserir(no *raiz, int chave){
  //cria um novo no
  if(raiz == NULL){
    no * novo;
    novo = (no*)malloc(sizeof(no));
    novo->chave = chave;
    novo->dir = NULL;
    novo->esq = NULL;
    return novo;

  }else{
    //se o valor do no eh maior que o valor atual, insere na sub arvore a direita
    if(chave > raiz->chave){
      raiz->dir = inserir(raiz->dir, chave);
    }else if (chave < raiz->chave){
      //se o valor do no eh menor que o valor atual, insere na sub arvore a esqueda
      raiz->esq = inserir(raiz->esq, chave);
    }
    return raiz;    
  }
}

void preorder(no *raiz){
  if(raiz != NULL){
    printf("[%d]", raiz->chave);
    preorder(raiz->esq);
    preorder(raiz->dir);
  }
}

void inorder(no *raiz){
  if(raiz != NULL){
    inorder(raiz->esq);
    printf("[%d]", raiz->chave);
    inorder(raiz->dir);
  }
}

void posorder(no *raiz){
  if(raiz != NULL){
    posorder(raiz->esq);
    posorder(raiz->dir);
    printf("[%d]", raiz->chave);
  }
}

no* remover(no *raiz, int chave){
  //caso base
  if(raiz==NULL){
    return raiz;
  }
  //verifica se o chave eh menor que a chave atual e procura na sub arvore a esquerda
  else if(chave < raiz->chave){
    raiz->esq = remover(raiz->esq, chave);
  }
  //verifica se a chave eh maior que a chave atual e procura na sub arvore a direita
  else if(chave > raiz->chave){
    raiz->dir = remover(raiz->dir, chave);
  }

  //se a chave for igual, esse sera o no a ser deletado
  else{
    //caso 1: no com um ou nenhum filho
    //se nao existir no a esquerda o no atual eh deletado e substituido pelo no a direita caso exista
    if(raiz->esq == NULL){
      no* auxiliar = raiz->dir;
      free(raiz);
      return auxiliar;

    //se nao existir no a direita o no atual eh deletado e substituido pelo no a esquerda caso exista 
    }else if(raiz->dir == NULL){
      no* auxiliar = raiz->esq;
      free(raiz);
      return auxiliar;
    }

    //caso 2: no com dois filhos
    else{
      //pega o sucessor
      no* auxiliar = sucessor(raiz);
      //substitui o valor do no atual pelo valor do sucessor
      raiz->chave = auxiliar->chave;
      //deleta o sucessor
      raiz->dir = remover(raiz->dir, auxiliar->chave);
    }
  }
  return raiz;
}

no* maior(no *raiz){
  //como os valores maiores ficam direita, procura-se ate o no mais a direita que eh o de maior valor
  while(raiz->dir != NULL){
    raiz = raiz->dir;
  }
  return raiz;
}

no* menor(no *raiz){
  //como os valores menores ficam a esquerda, procura-se ate o no mais a esquerda que eh o de menor valor 
  while(raiz->esq != NULL){
    raiz = raiz->esq;
  }
  return raiz;
}

int altura(no *raiz){
  //caso base
  if(raiz == NULL){
    return 0;
  
  }else{
    int altura_dir = altura(raiz->dir); //calcula a altura da sub arvore a direita
    int altura_esq = altura(raiz->esq); //calcula a altura da sub arvore a esquerda

    if(altura_dir > altura_esq){ //retorna altura da maior subarvore + 1 para a recursao
      return (altura_dir)+1;
    }else{
      return (altura_esq)+1;
    }
  }
}

int existe(no *raiz, int chave){
  //caso base, o no nao existe, retorna 0
  if(raiz==NULL){
    return 0;
  }else{
    //caso o no seja o que se procura retorna 1
    if(raiz->chave == chave){
      return 1;
    //se o no for maior que o no atual procura na subarvore a direita
    }else if(raiz->chave < chave){
      return existe(raiz->dir, chave);
     //se o no for menor que o no atual procura na subarvore a esquerda
    }else{
      return existe(raiz->esq, chave);
    }
  }
}

int quantidade_elementos(no *raiz){
  if(raiz != NULL){
    //calcula a quantidade de elementos de cada subarvore recursivamente ate o caso base de 1 unico elemento
    // soma mais 1 que eh a raiz
    return quantidade_elementos(raiz->dir) + quantidade_elementos(raiz->esq) + 1;
  }else{
    return 0;
  }
}

no* predecessor(no *raiz){
  if(raiz != NULL){
    //pega o maior valor da sub arvore a esquerda
    //sendo esse o valor que precede o valor do no em inorder
    raiz = maior(raiz->esq);
  }
  return raiz;
}

no* sucessor(no *raiz){
  if(raiz != NULL){
    //pega o menor valor da sub arvore a direita
    //sendo esse o o valor que sucede o valor do no em inorder
    raiz = menor(raiz->dir);
  }
  return raiz;
}