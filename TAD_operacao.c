#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "TAD_operacao.h"
#include "TAD_fila.h"
#include "TAD_serializacao.h"

void operacaoInsere(char *aux, Arvore *T, unsigned long long int num_campos, unsigned long long int id_campo, FILE *disco, unsigned long long int posicao){
	unsigned long long int j=0, k=0, i=0;
	i= posicao;
	//Criar uma variável do tipo "Registro", para receber o novo registro.
	Registro reg;
	reg.num_registros =num_campos;
	reg.registros= (char**) malloc((num_campos+1)*sizeof(char*));
	for(j=0; j<=num_campos; j++){
		reg.registros[j]=(char*) malloc(31*sizeof(char));
		for(k=0;k<31;k++)
			reg.registros[j][k]=0;
	}
	j=0;
	k=0;
	//Armazenar os campos do registro na variável.
	for(j=0; j<num_campos; j++){
		while(isblank(aux[i]))
			i++;
		while(!isblank(aux[i]) && aux[i]!='\n'){
			reg.registros[j][k]= aux[i];
			i++;
			k++;
		}
		k=0;
	}
	//Verificar qual registro é a chave do registro indexada na Árvore.
	char *end;
	reg.key= strtoull(reg.registros[id_campo], &end, 10);
	//Armazenar o registro no disco.
	reg.offset = ftell(disco);
	Serialize_registro(reg, disco, reg.offset);
	//Inserir o registro na Árvore.	
	insereRegistro(T, reg, disco);
	//Liberar a memória alocada.
	for(j=0; j<=num_campos; j++){
		free(reg.registros[j]);
	}
	free(reg.registros);
}

void operacaoBusca(char *aux, Arvore *T, FILE *output, unsigned long long int num_campos, FILE *disco, unsigned long long int ordem, unsigned long long int posicao){
	unsigned long long int j=0, result=0, i=0;
	i= posicao;
	//Criar uma variável para receber a chave a ser buscada.
	unsigned long long int busca_chave;
	char *key;
	key= (char*) malloc(31*sizeof(char));
	for(j=0; j<31; j++)
		key[j]=0;
	j=0;
	//Passar os espaços vazios entre a palavra "search" e a chave "key", no arquivo de entrada.	
	while(isblank(aux[i]))
		i++;
	//Armazenar a chave a ser buscada na variável "key".	
	while(!isblank(aux[i]) && aux[i]!='\n'){
		key[j]= aux[i];
		i++;
		j++;
	}
	char *end;
	//Transformar a chave em uma variável tipo "unsigned long long int" .	
	busca_chave= strtoull(key, &end, 10);
	//Realizar a busca pela chave no banco de dados.	
	result = busca(T->raiz, busca_chave, output, disco, ordem, num_campos, T);
	//Se não achou a chave na árvore, imprime "null".	
	if(result==0)
		fprintf(output, "null\n");
	free(key);
}

void executaOperacao(char *aux, char *operacao, Arvore *T, FILE *output, unsigned long long int num_campos, unsigned long long int id_campo, FILE *disco, unsigned long long int ordem){
	char *search="search", *add="add", *dump="dump";
	unsigned long long int i=0;
	//Verificar a operação solicitada.
	while(!isblank(aux[i]) && aux[i]!='\n' && aux[i]!=0){
		operacao[i]=aux[i];
		i++;
	}	
	operacao[i]=0;
	//Imprime no arquivo a operação que será executada.	
	if(strcmp(operacao, add)!=0)
		fprintf(output, "%s\n", operacao);
	//Se a operação for "dump", imprime a árvore no arquivo.	
	if(strcmp(operacao, dump)==0){
		No *n=NULL;
		imprimeArvore(T, n, output, disco);
	}
	//Se a operação for "add", verifica o novo registro e adiciona no banco de dados.
	if(strcmp(operacao, add)==0)
		operacaoInsere(aux, T, num_campos, id_campo, disco, i);
	//Se a operação for "search", realiza a busca pela chave solicitada.
	if(strcmp(operacao, search)==0)
		operacaoBusca(aux, T, output, num_campos, disco, ordem, i);
	//Imprime no arquivo a operação que foi executada.	
	if(strcmp(operacao, add)!=0)
		fprintf(output, "%s\n", operacao);
}

