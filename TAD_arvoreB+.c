#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "TAD_arvoreB+.h"
#include "TAD_fila.h"
#include "TAD_serializacao.h"

int teto(int x, int y){
	int q=0;
	q = (x % y) ? (x / y + 1) : (x / y);
	return q;
}

void alocaMemoriaNodo(No *nodo, unsigned long long int ordem){
	unsigned long long int k=0;
	//Alocar memória para todos os atributos do nó.
	nodo->posicao_filhos= (long int*) malloc((ordem + 1)*sizeof(long int)); 	
	nodo->chaves= (unsigned long long int*) malloc((ordem)*sizeof(unsigned long long int));
	nodo->posicao_registro= (long int*) malloc((ordem )*sizeof(long int));	
	//Inicializar todos os valores do nó.	
	nodo->folha= true;
	nodo->num_chaves=0;
	nodo->prox_folha=0;
	nodo->num_registros=0;
	nodo->offset=0;
	for(k=0;k<(ordem);k++){
		nodo->posicao_filhos[k]=-1;
		nodo->posicao_registro[k]= -1;
		nodo->chaves[k]=0;
	}
	nodo->posicao_filhos[k]=-1;
}

void desalocaMemoriaNodo(No *nodo, unsigned long long int ordem){
	free(nodo->posicao_registro);
	free(nodo->chaves);
	free(nodo->posicao_filhos);
	free(nodo);
}

void criaArvore(Arvore *T, FILE *disco){
	T->raiz= (No*) malloc(sizeof(No));
	alocaMemoriaNodo(T->raiz, T->ordem);
}

void imprimeArvore(Arvore *T, No *n, FILE *output, FILE *disco){
	unsigned long long int i=0;
	//Criar uma fila de nodos, que será utilizada na impressão das chaves.
	fila *impressao;
	impressao= (fila*) malloc(sizeof(fila));
	novaFila(impressao);
	//O nó raiz deve ser o primeiro a entrar na fila.	
	enfileira(impressao, T->raiz); 
	while(!filaVazia(impressao)){
			//Desenfileira o próximo nó da fila.
			n= desenfileira(impressao);
			//Se não for um nó folha, enfileirar todos os seus filhos.	
			if(n->folha==false){
				i=1;
				//Enfileirar cada filho do nodo n.
				while(i<=(n->num_chaves+1)){
					No* aux;	
					//Alocar memória e trazer o filho do nodo para a memória principal.
					aux = (No*) malloc(sizeof(No));
					alocaMemoriaNodo(aux, T->ordem);
					if(n->posicao_filhos[i]!=-1)						
						Deserialize(aux, disco, n->posicao_filhos[i], T->ordem);
					//Enfileira o filho do nodo.
					if(aux->num_chaves !=0)
						enfileira(impressao, aux);
					else
						desalocaMemoriaNodo(aux, T->ordem);	
					i++;
				}
			}
			//Imprimir todas as chaves do nó que foi desenfileirado.	
			for(i=1; i<=(n->num_chaves);i++)
				fprintf(output, "%llu,", n->chaves[i]);	
			fprintf(output, "\n");
			//Liberar a memória utilizada temporariamente pelo nodo desenfileirado.	
			if(n!=T->raiz)
				desalocaMemoriaNodo(n, T->ordem);
	}
	free(impressao->frente);
	free(impressao);
}

void reparteNodoOrdemImpar(No* nodo, No* parte1, No* parte2, unsigned long long int ordem){
	unsigned long long int j=0;
	//Se é um nó folha, a primeira chave da parte2 deve ser copia da chave que subiu para o nó pai. 
	if(parte2->folha==true){
		parte2->num_chaves= (unsigned long long int)teto(ordem, 2) -1; 
		parte2->num_registros= parte2->num_chaves;
		for(j=1; j<= (unsigned long long int)teto(ordem, 2)-1; j++){
			parte2->chaves[j]=parte1->chaves[j+(unsigned long long int)teto(ordem, 2)-1]; 
			parte2->posicao_registro[j]=parte1->posicao_registro[j+(unsigned long long int)teto(ordem, 2)-1];			
		}			
		parte1->num_chaves= ordem - parte2->num_chaves -1;
		parte1->num_registros= parte1->num_chaves;
	}	
	//Se é um nó interno, não é necessário fazer cópia da chave na parte2.	
	else{ 		
		parte2->num_chaves= (unsigned long long int)teto(ordem, 2) -2; 	
		parte2->num_registros= parte2->num_chaves;
		for(j=1; j<= (unsigned long long int)teto(ordem, 2)-2; j++){
			parte2->chaves[j]=parte1->chaves[j+(unsigned long long int)teto(ordem, 2)]; 
			parte2->posicao_registro[j]=parte1->posicao_registro[j+(unsigned long long int)teto(ordem, 2)]; 		
		}
		parte1->num_chaves= ordem - parte2->num_chaves -2; 
		parte1->num_registros= parte1->num_chaves;
		//Quando o nó dividido é interno, deve-se copiar os filhos para o novo nó.
		parte2->posicao_filhos[1]= -1;
		for(j=1; j< (unsigned long long int)teto(ordem, 2); j++) 
			parte2->posicao_filhos[j]=parte1->posicao_filhos[j+(unsigned long long int)teto(ordem, 2)];	
	}
}

void reparteNodoOrdemPar(No* nodo, No* parte1, No* parte2, unsigned long long int ordem){
	unsigned long long int j=0;
	//Se é um nó folha, a primeira chave da parte2 deve ser copia da chave que subiu para o nó pai. 
	if(parte2->folha==true){
		parte2->num_chaves= (unsigned long long int)teto(ordem, 2); 
		parte2->num_registros= parte2->num_chaves;
		for(j=1; j<= (unsigned long long int)teto(ordem, 2); j++){
			parte2->chaves[j]=parte1->chaves[j+(unsigned long long int)teto(ordem, 2)-1]; 
			parte2->posicao_registro[j]=parte1->posicao_registro[j+(unsigned long long int)teto(ordem, 2)-1]; 		
		}
		parte1->num_chaves= ordem - parte2->num_chaves -1; 
		parte1->num_registros= parte1->num_chaves;
	}
	//Se é um nó interno, não é necessário fazer cópia da chave na parte2.	
	else{
		parte2->num_chaves= (unsigned long long int)teto(ordem, 2) -1; 
		parte2->num_registros= parte2->num_chaves;
		for(j=1; j<= (unsigned long long int)teto(ordem, 2)-1; j++){
			parte2->chaves[j]=parte1->chaves[j+(unsigned long long int)teto(ordem, 2)]; 
			parte2->posicao_registro[j]=parte1->posicao_registro[j+(unsigned long long int)teto(ordem, 2)]; 		
		}
		parte1->num_chaves= parte2->num_chaves ; 
		parte1->num_registros= parte1->num_chaves;
		//Quando o nó dividido é interno, deve-se copiar os filhos para o novo nó.
		parte2->posicao_filhos[1]= -1;
		for(j=1; j<= (unsigned long long int)teto(ordem, 2); j++)
			parte2->posicao_filhos[j]=parte1->posicao_filhos[j+(unsigned long long int)teto(ordem, 2)]; 	
	}
}			

void reparteNodo(No *nodo, unsigned long long int indice, unsigned long long int ordem, FILE *disco){ 
	unsigned long long int j=0;
	No *parte1, *parte2;
	parte1 = (No*) malloc(sizeof(No));
	alocaMemoriaNodo(parte1, ordem);
	//Criar mais um filho para o nodo, que ficará com a segunda parte (da direita) do nodo cheio.
	parte2 = (No*) malloc(sizeof(No));
	alocaMemoriaNodo(parte2, ordem);	
	parte2->offset= ftell(disco);
	//parte1 recebe a primeira parte do filho cheio do nodo. 
	Deserialize(parte1, disco, nodo->posicao_filhos[indice], ordem);
	parte2->folha=parte1->folha;
	//Se a ordem da árvore é ímpar, o elemento do meio [m/2] do nó cheio vai para o nó pai.
	if(ordem%2!=0)
		reparteNodoOrdemImpar(nodo, parte1, parte2, ordem);
	//Se a ordem da árvore for par, o elemento [teto(m/2)] vai para o nó pai.
	else
		reparteNodoOrdemPar(nodo, parte1, parte2, ordem);
	//O nodo pai recebe o novo filho (segunda parte da partição do nó cheio).	
	for(j=(nodo->num_chaves + 1); j>=indice+1; j--)
		nodo->posicao_filhos[j+1]=nodo->posicao_filhos[j];
	nodo->posicao_filhos[indice+1]= parte2->offset;
	//O nodo pai recebe a chave e o registro do novo filho (segunda parte da partição do nó cheio).			
	for(j=nodo->num_chaves; j>=indice; j--){
		nodo->chaves[j+1]=nodo->chaves[j];
		nodo->posicao_registro[j+1]=nodo->posicao_registro[j];
	}	
	nodo->chaves[indice]= parte1->chaves[(unsigned long long int)teto(ordem, 2)]; 
	nodo->posicao_registro[indice]= parte1->posicao_registro[(unsigned long long int)teto(ordem, 2)];
	nodo->num_chaves++;
	//Retira os registros do nó folha que passou a ser nó interno.
	nodo->num_registros=0;
	//Se o nó cheio era folha, colocar um ponteiro da folha 1 para a folha 2.
	if(parte1->folha==true)
		parte1->prox_folha= parte2->offset;
	//Armazenar os nodos no disco.
	Serialize(parte2, disco, parte2->offset, ordem);
	Serialize(parte1, disco, parte1->offset, ordem);
	fseek(disco, 0, SEEK_END);
	//Liberar a memória utilizada temporariamente pela função.	
	desalocaMemoriaNodo(parte1, ordem);	
	desalocaMemoriaNodo(parte2, ordem);

}

void insereArvoreNaoCheia(Arvore *T, No *nodo, Registro k, unsigned long long int ordem, FILE *disco){
	unsigned long long int i=0;
	i= nodo->num_chaves;
	//Se o nó é folha, adiciona o registro no nó.
	if(nodo->folha==true){
			//Verifica a posição em que o registro deve ser inserido.
			while(i>=1 && k.key<(nodo->chaves[i])){
				nodo->chaves[i+1]=nodo->chaves[i]; 
				nodo->posicao_registro[i+1]= nodo->posicao_registro[i];
				i--;
			}
			//Armazenar o registro no nodo.
			nodo->chaves[i+1]= k.key;
			nodo->posicao_registro[i+1]= k.offset;
			nodo->num_registros++;
			nodo->num_chaves++;	
			//Armazenar o nodo modificado no disco.
			if(nodo!=T->raiz)
				Serialize(nodo, disco, nodo->offset, ordem);
	}
	//Se o nó é interno, caminha mais um nível na árvore. 
	else{
			//Verificar o filho em que o registro deve ser inserido.
			while(i>=1 && k.key<(nodo->chaves[i]))
				i--;
			i++;
			//Armazenar o filho na memória primária.
			No* filho;
			filho=  (No*) malloc(sizeof(No));
			alocaMemoriaNodo(filho, ordem);	
			Deserialize(filho, disco, nodo->posicao_filhos[i], ordem);		
			//Se o filho do nó estiver cheio, repartir o nó cheio.
			if(filho->num_chaves == (ordem -1)){
				reparteNodo(nodo, i, ordem, disco); 
				//Armazenar o nodo modificado no disco.
				if(nodo!=T->raiz)
					Serialize(nodo, disco, nodo->offset, ordem);
				//Verificar qual das duas repartições o registro deve ser inserido.  
				if (k.key > nodo->chaves[i])
					i++;		
				//Atualizar o nodo filho após a repartição.
				Deserialize(filho, disco, nodo->posicao_filhos[i], ordem);
				//Inserir o registro no filho.
				insereArvoreNaoCheia(T, filho, k, ordem, disco);	
			}
			//Se o filho do nó não estiver cheio.
			else 
				insereArvoreNaoCheia(T, filho, k, ordem, disco);	
			//Liberar a memória utilizada temporariamente pelo nó filho.	
			desalocaMemoriaNodo(filho, ordem);
	}
}

void insereRegistro(Arvore *T, Registro k, FILE *disco){
	No *r;
	r= T->raiz;
	//Se a raiz da arvore estiver cheia, deve-se criar uma raiz nova.
	if(r->num_chaves == (T->ordem) - 1){
			//Armazenar a raiz antiga no disco.
			r->offset = ftell(disco);
			Serialize(r, disco, r->offset, T->ordem);
			//Armazenar a raiz nova na memória primária.
			No *s;
			s= (No*) malloc(sizeof(No));
			alocaMemoriaNodo(s, T->ordem);
			T->raiz = s;
			s->folha = false;
			s->num_chaves = 0;
			s->num_registros=0;
			s->posicao_filhos[1]=r->offset;
			reparteNodo(s, 1, T->ordem, disco);
			insereArvoreNaoCheia(T, s, k, T->ordem, disco);
			//Retirar a raiz antiga da memória primária.
			desalocaMemoriaNodo(r, T->ordem);
	}
	//Se a raiz não estiver cheia, inserir o registro na árvore.
	else
		insereArvoreNaoCheia(T, r, k, T->ordem, disco);

}

void imprimeRegistro(Registro reg, FILE *output, FILE *disco){
	unsigned long long int i=0;
	for(i=0; i< (reg.num_registros); i++){
		if(i!=0)
			fprintf(output, "\t");
		fprintf(output, "%s", reg.registros[i]);
	}
	fprintf(output, "\n");

}

int busca(No *nodo, unsigned long long int chave_buscada, FILE *output, FILE *disco, unsigned long long int ordem, unsigned long long int num_campos, Arvore *T){
	unsigned long long int i=1, j=0, k=0;
	//Verificar o filho em que a chave deve ser encontrada.
	while(i<=(nodo->num_chaves) && chave_buscada >= nodo->chaves[i])
		i++;
	//Se o nodo é nó folha, ou o registro é encontrado neste nó ou o registro não se encontra na árvore.	
	if(nodo->folha==true){
		i--;
		//Se achou o registro no nodo folha.
		if(i<=(nodo->num_chaves) && chave_buscada== nodo->chaves[i]){
				//Obter o registro no disco e imprimir o registro no arquivo de saída.	
				Registro reg;
				reg.num_registros =num_campos;
				reg.registros= (char**) malloc((num_campos+1)*sizeof(char*));
				for(j=0; j<=num_campos; j++){
					reg.registros[j]=(char*) malloc(31*sizeof(char));
					for(k=0;k<31;k++)
						reg.registros[j][k]=0;
				}
				Deserialize_registro(&reg, disco, nodo->posicao_registro[i]);
				imprimeRegistro(reg, output, disco);
				//Liberar a memória ocupada temporariamente pelo registro.
				for(j=0; j<=num_campos; j++){
						free(reg.registros[j]);
					}
				free(reg.registros);
				return 1;
		}
		//Se não achou o registro no nodo folha.
		else
			return 0;	
	}
	//Se o nó não tem filho na direção da busca, o registro não existe na árvore.	
	else if(nodo->posicao_filhos[i]== -1)
		return 0;	
	//Se o nó tem filho na direção da busca, continuar a busca no filho.
	No* filho;
	filho =  (No*) malloc(sizeof(No));
	alocaMemoriaNodo(filho, ordem);
	Deserialize(filho, disco, nodo->posicao_filhos[i], ordem);
	int resultadoBusca=0;
	resultadoBusca= busca(filho, chave_buscada, output, disco, ordem, num_campos, T);
	//Liberar a memória utilizada na função.	
	desalocaMemoriaNodo(filho, ordem);
	//Retorna o resultado da busca: 1-> achou ou 0-> não achou.	
	return resultadoBusca;
}	


