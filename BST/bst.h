#ifndef BST_H
#define BST_H

typedef struct no{
  int chave;
  struct no *dir;
  struct no *esq;
}no;

//op 1
no* inserir(no *raiz, int chave);
//op 2
void preorder(no *raiz);
//op 3
void inorder(no *raiz);
//op 4
void posorder(no *raiz);
//op 5
no* remover(no *raiz, int chave);
//op 6
no* maior(no *raiz);
//op 7
no* menor(no *raiz);
//op 8
int altura(no *raiz);
//op 9
int quantidade_elementos(no* raiz);
//op 10
int existe(no* raiz, int chave); //retorna 1 se existir e 0 se nao existir
//op 11
no *predecessor(no* raiz);
//op12
no* sucessor(no* raiz);

#endif