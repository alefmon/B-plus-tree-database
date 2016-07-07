#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "TAD_fila.h"

int filaVazia(fila *queue){
	if(queue->tras==queue->frente)
		return 1;
	return 0;
}

void novaFila(fila *queue){
	queue->frente = malloc(sizeof(celula));
	queue->tras= queue->frente;
	queue->frente->prox= NULL;
}

void enfileira(fila *queue, No* new_item){
	queue->tras->prox= malloc(sizeof(celula));
	queue->tras = queue->tras->prox;
	queue->tras->item = new_item;
	queue->tras->prox = NULL; 
}

No* desenfileira(fila *queue){
	No* item;
	pont aux = queue->frente;
	queue->frente= queue->frente->prox;
	item= queue->frente->item;
	free(aux);
	return item;
}

