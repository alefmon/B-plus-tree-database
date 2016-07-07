#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "TAD_arvoreB+.h"
#ifndef SERIALIZE_H
#define SERIALIZE_H

//A função realiza a serialização de uma string em bytes e escreve os bytes no disco.
void Serialize_string(char *string, long int tam_string, FILE *disco, long int offset);
//A função faz a leitura de uma sequência de bytes no disco e desserializa os bytes para obter o conteúdo de uma string.
void Deserialize_string(char *string, long int tam_string, FILE *disco, long int offset);
//A função realiza a serialização de um vetor em bytes e escreve os bytes no disco.
void Serialize_vetor(unsigned long long int *vetor, unsigned long long int tam_vetor, FILE *disco, long int offset);
//A função faz a leitura de uma sequência de bytes no disco e desserializa os bytes para obter o conteúdo de um vetor.
void Deserialize_vetor(unsigned long long int *vetor, unsigned long long int tam_vetor, FILE *disco, long int offset);
//A função realiza a serialização de um nó em bytes e escreve os bytes no disco.
void Serialize (No *nodo , FILE *disco, long int offset, unsigned long long int ordem);
//A função faz a leitura de uma sequência de bytes no disco e desserializa os bytes para obter o conteúdo de um nó.
void Deserialize (No *nodo, FILE *disco, long int offset, unsigned long long int ordem);
//A função realiza a serialização de um registro em bytes e escreve os bytes no disco.
void Serialize_registro(Registro k, FILE *disco, long int offset);
//A função faz a leitura de uma sequência de bytes no disco e desserializa os bytes para obter o conteúdo de um registro.
void Deserialize_registro(Registro *k, FILE *disco, long int offset);



#endif
