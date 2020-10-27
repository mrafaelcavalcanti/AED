#include <stdio.h>
#include <stdlib.h>
#include "avl.h"
#include <string.h>

extern char* strdup(const char*);



void inicializar(arvore *raiz){
  *raiz = NULL;
}



int inicializarTabela(tabela *tab){
    inicializar(&tab->indices);
    tab->arquivo_dados = fopen("dados.txt", "a+");
    tab->indices = carregar_arquivo("indices.dat", tab->indices);
    if(tab->arquivo_dados != NULL){
        return 1;
    }else{
        return 0;
    }
}



void finalizar(tabela *tab){
    fclose(tab->arquivo_dados);
    salvar_arquivo("indices.dat", tab->indices);
}



void adicionarLivro(tabela *tab, dado *livro){
    if(tab->arquivo_dados != NULL){
        int cresceu;

        fseek(tab->arquivo_dados, 0L, SEEK_END);

        tipo_dado * novo = (tipo_dado *) malloc(sizeof(tipo_dado));
        novo->chave = livro->codigo;
        novo->indice = ftell(tab->arquivo_dados);

        fprintf(tab->arquivo_dados, "%s|%s|%s|%d|", livro->titulo, livro->autor, livro->isbn, livro->codigo);

        //fwrite(livro, sizeof(dado), 1, tab->arquivo_dados);
        tab->indices = adicionar(novo, tab->indices, &cresceu);
    }
}



dado *ler_dados(){
    dado *novo = (dado *) malloc(sizeof(dado));
    char * buffer = (char *) malloc(256 * sizeof(char));

    getchar();

    printf("titulo: ");
    fgets(buffer, 255, stdin);
    tirar_enter(buffer);
    novo->titulo = strdup(buffer);


    printf("autor: ");
    fgets(buffer, 255, stdin);
    tirar_enter(buffer);
    novo->autor = strdup(buffer);

    
    printf("ISBN: ");
    fgets(buffer, 255, stdin);
    tirar_enter(buffer);
    novo->isbn = strdup(buffer);

    
    printf("codigo: ");
    scanf("%d", &novo->codigo);
    free(buffer);


    return novo;
}



void tirar_enter(char *string){
  string[strlen(string)-1] = '\0';
}



void salvar_arquivo(char *nome, arvore a){
    FILE *arq;
    arq = fopen(nome, "wb");
    if(arq != NULL){
        salvar_auxiliar(a, arq);
        fclose(arq);
    }
}



void salvar_auxiliar(arvore raiz, FILE *arq){
    if(raiz != NULL){
        fwrite(raiz->dado, sizeof(tipo_dado), 1, arq);
        salvar_auxiliar(raiz->esq, arq);
        salvar_auxiliar(raiz->dir, arq);
    }
}



arvore carregar_arquivo(char *nome, arvore a){
    FILE *arq;
    arq = fopen(nome, "rb");
    tipo_dado * temp;
    if(arq != NULL){
        int cresceu;
        temp = (tipo_dado *) malloc(sizeof(tipo_dado));
        while(fread(temp, sizeof(tipo_dado), 1, arq)){
          a = adicionar(temp, a, &cresceu);
          temp = (tipo_dado *) malloc(sizeof(tipo_dado));
        }
        fclose(arq);
    }
    return a;
}


void buscar_livro(int codigo, arvore raiz, tabela *tab){
    if(raiz!=NULL){
        if(raiz->dado->chave == codigo){
            imprimir_elemento(raiz, tab);
        }else if (raiz->dado->chave < codigo){
            buscar_livro(codigo, raiz->dir, tab);
        }else{
            buscar_livro(codigo, raiz->esq, tab);
        }
    }
}




//a partir daqui começa o codigo da avl


arvore adicionar(tipo_dado *valor, arvore raiz, int *cresceu){
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
    if(valor->chave > raiz->dado->chave){
      raiz->dir = adicionar(valor, raiz->dir, cresceu);
      if(*cresceu){
        switch(raiz->fb){
          case -1:    
            raiz->fb = 0;
            *cresceu = 0;
            break;
          case 0:   
            raiz->fb = 1;
            *cresceu = 1;
            break;
          case 1:   
            *cresceu = 0;
            return rotacionar(raiz);
        }
      }
    }else if(valor->chave < raiz->dado->chave){
      raiz->esq = adicionar(valor, raiz->esq, cresceu);
      if(*cresceu){
        switch(raiz->fb){
          case 1:   
            raiz->fb = 0;
            *cresceu = 0;
            break;
          case 0:   
            raiz->fb = -1;
            *cresceu = 1;
            break;
          case -1:  
            *cresceu = 0;
            return rotacionar(raiz); 
        }
      }
    }
  }
  return raiz;
}



arvore remover(int valor, arvore raiz, int *diminuiu){
  if(raiz == NULL){
    *diminuiu = 0;
    return raiz;
  }else{
    if(valor > raiz->dado->chave){
      raiz->dir = remover(valor, raiz->dir, diminuiu);
      if(*diminuiu){
        switch(raiz->fb){
          case 1:   
            raiz->fb = 0;
            *diminuiu = 1;
            break;
          case 0:   
            raiz->fb = -1;
            *diminuiu = 0;
            break;
          case -1:  
            if(raiz->esq->fb == 0){
              *diminuiu = 0;
            }
            return rotacionar(raiz);
        }
      }
    }else if(valor < raiz->dado->chave){
      raiz->esq = remover(valor, raiz->esq, diminuiu);
      if(*diminuiu){
        switch(raiz->fb){
          case -1:  
            raiz->fb = 0;
            *diminuiu = 1;
            break;
          case 0:   
            raiz->fb = 1;
            *diminuiu = 0;
            break;
          case 1:
            if(raiz->dir->fb == 0){
              *diminuiu = 0;
            }
            return rotacionar(raiz);
        }
      }
    }else{
      if(raiz->dir != NULL && raiz->esq != NULL){  
        arvore aux;
        aux = raiz->dir;
        while(aux->esq!=NULL){  
          aux = aux->esq;
        }
        raiz->dado = aux->dado;
        raiz->dir = remover(aux->dado->chave, raiz->dir, diminuiu); 
        if(*diminuiu){
        switch(raiz->fb){
          case 1:   
            raiz->fb = 0;
            *diminuiu = 1;
            break;
          case 0:   
            raiz->fb = -1;
            *diminuiu = 0;
            break;
          case -1:  
            if(raiz->esq->fb == 0){
              *diminuiu = 0;
            }
                        
            return rotacionar(raiz);
          }
        }
      }else{
        arvore aux_2 = raiz;
        if(raiz->dir != NULL){
          raiz = raiz->dir;
        }else if(raiz->esq != NULL){
          raiz = raiz->esq;
        }else{
          raiz = NULL;
        }
        free(aux_2);
        *diminuiu = 1;
        return raiz;
      }
    }
    return raiz;
  }
}



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



arvore rotacao_simples_direita(arvore raiz){
  arvore p = raiz;
  arvore u = raiz->esq;
  p->esq = u->dir;
  u->dir = p;

  return u;
}


arvore rotacao_simples_esquerda(arvore raiz){
  arvore p = raiz;
  arvore u = raiz->dir;
  p->dir = u->esq;
  u->esq = p;

  return u;
}


arvore rotacao_dupla_direita(arvore raiz){
  raiz->esq = rotacao_simples_esquerda(raiz->esq);
  return rotacao_simples_direita(raiz);
}


arvore rotacao_dupla_esquerda(arvore raiz){
  raiz->dir = rotacao_simples_direita(raiz->dir);
  return rotacao_simples_esquerda(raiz);
}



void imprimir(arvore raiz) {
	printf("(");
	if(raiz != NULL) {
		//imprimir_elemento(raiz, tab);
		imprimir(raiz->esq);
		imprimir(raiz->dir);
	}
	printf(")");
}



void imprimir_elemento(arvore raiz, tabela * tab) {
	dado * temp = (dado *) malloc (sizeof(dado));
    int contador_barras = 0;
    char *buffer = malloc(sizeof(char)*256);

	fseek(tab->arquivo_dados, raiz->dado->indice, SEEK_SET);
    
	while(1){
        fscanf(tab->arquivo_dados, "%c", buffer);
        if(buffer[0] == '|'){
            contador_barras++;
            if(contador_barras==4){
                printf("\n");
                break;
            }else{
                printf(", ");
                continue;
            }
        }
        printf("%c", buffer[0]);
    }
	//printf("[%d, %s, %s, %s ]\n", raiz->dado->chave, temp->titulo, temp->autor, temp->isbn);
	//free(temp);
    free(buffer);
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



tipo_dado * maior_elemento(arvore raiz) {
	if(raiz == NULL)
			return NULL;
	if(raiz->dir == NULL)
			return raiz->dado;
	else
			return maior_elemento(raiz->dir);
}



tipo_dado * menor_elemento(arvore raiz) {
	if(raiz == NULL)
			return NULL;
	if(raiz->esq == NULL)
			return raiz->dado;
	else
			return menor_elemento(raiz->esq);
}



void pre_order(arvore raiz, tabela *tab) {
	if(raiz != NULL) {
		imprimir_elemento(raiz, tab);
		pre_order(raiz->esq, tab);
		pre_order(raiz->dir, tab);
	}
}

void pos_order(arvore raiz, tabela *tab) {
	if(raiz != NULL) {
		pos_order(raiz->esq, tab);
		pos_order(raiz->dir, tab);
		imprimir_elemento(raiz, tab);
	}
}

void in_order(arvore raiz, tabela *tab) {
	if(raiz != NULL) {
		in_order(raiz->esq, tab);
		imprimir_elemento(raiz, tab);
		in_order(raiz->dir, tab);
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
    //printf("%d", raiz->dado);
    display(raiz->esq, nivel+1);
  }
}