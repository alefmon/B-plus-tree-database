#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "TAD_arvoreB+.h"
#ifndef FILA_H
#define FILA_H

typedef struct Celula *pont;
//A estrutura representa cada elemento de uma fila.
typedef struct Celula{
	No* item;
	pont prox;
}celula;
//A estrutura representa uma fila de nós.
typedef struct{
	celula* frente;
	celula* tras;
	unsigned long long int tamanho;
}fila;
//A função verifica se a fila está ou não vazia.
int filaVazia(fila *queue);
//A função cria uma nova fila vazia.
void novaFila(fila *queue);
//A função insere um novo nó na fila.
void enfileira(fila *queue, No* new_item);
//A função retira da fila o próximo nó.
No* desenfileira(fila *queue);

#endif
