#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "TAD_serializacao.h"

void Serialize_string(char *string, long int tam_string, FILE *disco, long int offset){
	unsigned char aux[tam_string];
	long int i=0;
	for(i=0; i< tam_string; i++)
		aux[i]=string[i];
	//Ir para a posição do disco onde a string deve ser escrita e serializar a string.	
	fseek (disco, offset, SEEK_SET);
	fwrite (&(aux[0]) , sizeof(unsigned char), tam_string, disco);
}

void Deserialize_string(char *string, long int tam_string, FILE *disco, long int offset){
	unsigned char aux[tam_string];
	long int i=0;
	//Ir para a posição do disco onde a string foi escrita e desserializar a string.
	fseek (disco, offset, SEEK_SET);	
	fread (&(aux[0]) , sizeof(unsigned char), tam_string, disco);
	for(i=0; i< tam_string; i++)
		string[i]= aux[i];
}

void Serialize_vetor(unsigned long long int *vetor, unsigned long long int tam_vetor, FILE *disco, long int offset){
	unsigned long long int aux[tam_vetor+1];
	long int i=0;
	for(i=0; i<= tam_vetor; i++)
		aux[i]=vetor[i];
	//Ir para a posição do disco onde o vetor deve ser escrito e serializar o vetor.
	fseek (disco, offset, SEEK_SET);
	fwrite (&(aux[0]) , sizeof(unsigned long long int), tam_vetor+1, disco);
}

void Deserialize_vetor(unsigned long long int *vetor, unsigned long long int tam_vetor, FILE *disco, long int offset){
	unsigned long long int aux[tam_vetor+1];
	long int i=0;
	//Ir para a posição do disco onde a string foi escrita e desserializar a string.	
	fseek (disco, offset, SEEK_SET);	
	fread (&(aux[0]) , sizeof(unsigned long long int), tam_vetor+1, disco);
	for(i=0; i<= tam_vetor; i++)
		vetor[i]= aux[i];
}

void Serialize (No *nodo , FILE *disco, long int offset, unsigned long long int ordem){
	unsigned long long int metadata[5]={0};
	metadata[0] = nodo->num_chaves; 
	metadata[1] = nodo->folha;
	metadata[2] = nodo->prox_folha;
	metadata[3] = nodo->offset;
	metadata[4] = nodo->num_registros;
	//Ir para a posição do disco onde o nó deve ser escrito e serializar o nó.
	fseek (disco, offset, SEEK_SET);
	fwrite (&(metadata [0]) , sizeof(unsigned long long int), 5, disco);
	//Serializar os vetores presentes no nó.
	Serialize_vetor(nodo->chaves, ordem-1 , disco, ftell(disco));
	Serialize_vetor((unsigned long long int*)nodo->posicao_filhos, ordem, disco, ftell(disco));
	Serialize_vetor((unsigned long long int*)nodo->posicao_registro, ordem-1, disco, ftell(disco));
	//Voltar o cursor para a posição atual do arquivo.
	fseek(disco,0, SEEK_END);
}

void Deserialize (No *nodo, FILE *disco, long int offset, unsigned long long int ordem){
	unsigned long long int metadata [5]={0};
	//Ir para a posição do disco onde o nó foi escrito e desserializar o nó.
	fseek (disco ,  offset, SEEK_SET);
	fread (&(metadata[0]) , sizeof(unsigned long long int), 5, disco);
	nodo->num_chaves = metadata[0];
	nodo->folha = (int) metadata[1];
	nodo->prox_folha = metadata[2];
	nodo->offset = metadata[3];
	nodo->num_registros = metadata[4];
	//Desserializar os vetores presentes no conteúdo do nó.
	Deserialize_vetor(nodo->chaves, ordem-1, disco, ftell(disco));
	Deserialize_vetor((unsigned long long int*)nodo->posicao_filhos, ordem, disco, ftell(disco));
	Deserialize_vetor((unsigned long long int*)nodo->posicao_registro, ordem-1 , disco, ftell(disco));
	//Voltar o cursor para a posição atual do arquivo.
	fseek(disco,0, SEEK_END);
}

void Serialize_registro(Registro k, FILE *disco, long int offset){
	unsigned long long int metadata [3]={0};
	unsigned long long int i=0;
	metadata[0] = k.offset; 
	metadata[1] = k.key;
	metadata[2] = k.num_registros;
	//Ir para a posição do disco onde o registro deve ser escrito e serializar o registro.
	fseek (disco, offset, SEEK_SET);
	fwrite (&(metadata [0]) , sizeof(unsigned long long int), 3, disco);
	//Serializar cada um dos campos do registro.
	for(i=0; i< (k.num_registros); i++)
		Serialize_string(k.registros[i], 30, disco, ftell(disco));
	fseek(disco,0, SEEK_END);


}

void Deserialize_registro(Registro *k, FILE *disco, long int offset){
	unsigned long long int metadata [3]={0};
	unsigned long long int i=0;
	//Ir para a posição do disco onde o registro foi escrito e desserializar o registro.
	fseek (disco, offset, SEEK_SET);
	fread (&(metadata [0]) , sizeof(unsigned long long int), 3, disco);
	(*k).offset = metadata[0]; 	
	(*k).key = metadata[1];
	(*k).num_registros = metadata[2];
	//Desserializar cada um dos campos do registro.
	for(i=0; i< ((*k).num_registros); i++)
		Deserialize_string((*k).registros[i], 30 , disco, ftell(disco));
	//Voltar o cursor para a posição atual do arquivo.
	fseek(disco,0, SEEK_END);

}


