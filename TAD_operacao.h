#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "TAD_arvoreB+.h"
#ifndef OPERACAO_H
#define OPERACAO_H

//A função realiza a operação de inserção de um registro na árvore, quando esta é a operação solicitada na entrada.
void operacaoInsere(char *aux, Arvore *T, unsigned long long int num_campos, unsigned long long int id_campo, FILE *disco, unsigned long long int posicao);
//A função realiza a operação de busca de um registro na árvore, quando esta é a operação solicitada na entrada.
void operacaoBusca(char *aux, Arvore *T, FILE *output, unsigned long long int num_campos, FILE *disco, unsigned long long int ordem, unsigned long long int posicao);
//A função realiza a operação de impressão das chaves de cada nó da árvore, quando esta é a operação solicitada na entrada.
void executaOperacao(char *aux, char *operacao, Arvore *T, FILE *output, unsigned long long int num_campos, unsigned long long int id_campo, FILE *disco, unsigned long long int ordem);

#endif
