#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "TAD_arvoreB+.h"
#include "TAD_fila.h"
#include "TAD_serializacao.h"
#include "TAD_operacao.h"

int main(int argc, char const *argv[]){
	
	unsigned long long int i=0, ordem=0, id_campo=0, num_campos=0;
	FILE *input, *output, *disco;
	//Verificar os argumentos passados pelo terminal.
	input= fopen(argv[2], "rb");
	output= fopen(argv[1], "wb");
	char *end;
	ordem = strtoull(argv[3], &end, 10);
	num_campos= strtoull(argv[4], &end, 10);
	id_campo= strtoull(argv[5], &end, 10);
	//O arquivo temporário "disco" será utilizado como memória secundária.	
	disco = tmpfile();
	//Criar a árvore B+, que armazena o banco de dados.
	Arvore *tree;
	tree= (Arvore*) malloc(sizeof(Arvore));
	tree->ordem= ordem;
	criaArvore(tree, disco);
	//Para cada linha do arquivo de entrada, executar a operação solicitada.	
	while(1){
		char *operacao;
		operacao= (char*) malloc(10*sizeof(char));
		for(i=0; i<10; i++)
			operacao[i]=0;
		char *aux;
		aux= (char*) malloc((32*(num_campos+1))*sizeof(char));
		for(i=0; i<32*(num_campos + 1); i++)
			aux[i]=0;
		if(fgets(aux, (32*num_campos+1), input)==NULL){
			free(aux);
			free(operacao);
			break;
		}	
		executaOperacao(aux, operacao, tree, output, num_campos, id_campo, disco, ordem);
		free(aux);
		free(operacao);
	}	
	desalocaMemoriaNodo(tree->raiz, tree->ordem);
	free(tree);
	fclose(disco);
	fclose(input);
	fclose(output);
	return 0;
}