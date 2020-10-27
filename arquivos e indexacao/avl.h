#ifndef AVL_H
#define AVL_H



typedef struct livro{
    char *titulo;
    char *autor;
    char *isbn;
    int codigo;
}dado;



typedef struct indice{
  int chave;
  int indice;
}tipo_dado;



typedef struct no_avl{
    tipo_dado *dado;
    int fb;
    struct no_avl *esq;
    struct no_avl *dir;
}no_avl;



typedef no_avl * arvore;



typedef struct tabela{
    FILE *arquivo_dados;
    arvore indices;
}tabela;




	

void inicializar(arvore *raiz);
arvore adicionar (tipo_dado *valor, arvore raiz, int *cresceu);

int altura(arvore raiz);
int maior(int a, int b);
tipo_dado * maior_elemento(arvore raiz);
tipo_dado * menor_elemento(arvore raiz);
void pre_order(arvore raiz, tabela *tab);
void pos_order(arvore raiz, tabela *tab);
void in_order(arvore raiz, tabela *tab);
arvore remover (int valor, arvore raiz, int *diminuiu);
void imprimir_elemento(arvore raiz, tabela *tab);


arvore rotacionar(arvore raiz);
arvore rotacao_simples_direita(arvore raiz);
arvore rotacao_simples_esquerda(arvore raiz);
arvore rotacao_dupla_direita(arvore raiz);
arvore rotacao_dupla_esquerda(arvore raiz);

void salvar_arquivo(char *nome, arvore a);
void salvar_auxiliar(arvore raiz, FILE *arq);
arvore carregar_arquivo(char *nome, arvore a);
dado * ler_dados();
void tirar_enter(char *string);
int inicializarTabela(tabela *tab);
void finalizar (tabela *tab);
void adicionarLivro(tabela *tab, dado *livro);
void buscar_livro(int codigo, arvore raiz, tabela *tab);




#endif