#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

int main(int argc, char *argv[]) {
  no * arvore;
  no * aux;
  int inteiro;

  arvore = NULL;
  int opcao;
  do{
    scanf("%d", &opcao);
    switch(opcao){
    int chave;
      case 1:
      scanf("%d", &chave);
      arvore = inserir(arvore, chave);
      break;

      case 2:
      preorder(arvore);
      break;

      case 3:
      inorder(arvore);
      break;

      case 4:
      posorder(arvore);
      break;

      case 5:
      scanf("%d", &chave);
      remover(arvore, chave);
      break;

      case 6:
      aux = maior(arvore);
      printf("%d", aux->chave);
      break;

      case 7:
      aux = menor(arvore);
      printf("%d", aux->chave);
      break;

      case 8:
      inteiro = altura(arvore);
      printf("%d", inteiro);
      break;

      case 9:
      inteiro = quantidade_elementos(arvore);
      printf("%d", inteiro);
      break;

      case 10:
      scanf("%d", &chave);
      inteiro = existe(arvore, chave);
      printf("%d", inteiro);
      break;

      case 11:
      aux = predecessor(arvore);
      printf("%d", aux->chave);
      break;
      
      case 12:
      aux = sucessor(arvore);
      printf("%d", aux->chave);
      break;

      default:
      ;      
  
    }
  }while(opcao != 99);
  return 0;
}