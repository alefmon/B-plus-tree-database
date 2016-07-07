#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#ifndef BTREE_H
#define BTREE_H

//A estrutura representa uma variável booleana.
typedef int bool;
#define true 1
#define false 0
//A estrutura representa um registro.
typedef struct{
	long int offset;
	unsigned long long int key;
	unsigned long long int num_registros;
	char **registros;
}Registro;

typedef struct Nodo *apontador;
//A estrutura representa um nó e o conteúdo inserido nele.
typedef struct Nodo{
	unsigned long long int num_chaves;
	unsigned long long int num_registros;
	unsigned long long int *chaves;
	long int prox_folha;
	bool folha; 
	//posição do vetor no disco.
	long int offset;
	//vetor com as posições dos filhos no disco.
	long int *posicao_filhos;	
	//vetor com a posição dos registros no arquivo.
	long int *posicao_registro;
}No;
//A estrutura representa uma árvore B+.
typedef struct{
	No *raiz;
	unsigned long long int ordem;	
}Arvore;
//A função retorna o teto da divisão (x/y).
int teto(int x, int y);
//A função realiza a alocação dinâmica para todos os atributos de um nó e inicializa os valores do mesmo.
void alocaMemoriaNodo(No *nodo, unsigned long long int ordem);
//A função desaloca a memória alocada para o nó.
void desalocaMemoriaNodo(No *nodo, unsigned long long int ordem);
//A função cria uma árvore B+ vazia.
void criaArvore(Arvore *T, FILE *disco);
//A função realiza a impressão em largura das chaves presentes na árvore.
void imprimeArvore(Arvore *T, No *n, FILE *output, FILE *disco);
//A função é utilizada dentro da função "reparteNodo", quando a ordem da árvore é um número ímpar.
void reparteNodoOrdemImpar(No* nodo, No* parte1, No* parte2, unsigned long long int ordem);
//A função é utilizada dentro da função "reparteNodo", quando a ordem da árvore é um número par.
void reparteNodoOrdemPar(No* nodo, No* parte1, No* parte2, unsigned long long int ordem);
//A função reparte um nó em dois, quando o nó está cheio.
void reparteNodo(No *nodo, unsigned long long int indice, unsigned long long int ordem, FILE *disco);
//A função auxilia a função "insereRegistro" na inserção de um novo registro na árvore, quando esta não está cheia.
void insereArvoreNaoCheia(Arvore *T, No *nodo, Registro k, unsigned long long int ordem, FILE *disco);
//A função insere um novo registro na árvore.
void insereRegistro(Arvore *T, Registro k, FILE *disco);
//A função imprime todos os campos de um registro no arquivo de saída.
void imprimeRegistro(Registro reg, FILE *output, FILE *disco);
//A função realiza a busca de um registro na árvore.
int busca(No *nodo, unsigned long long int chave_buscada, FILE *output, FILE *disco, unsigned long long int ordem, unsigned long long int num_campos, Arvore *T);


#endif
