#include "auxiliar.h"

/* mostra a hash */
void print(unsigned char* hashf){
	int i;
	for(i=0; i< SHA256_DIGEST_LENGTH; i++){
		printf("%02x", hashf[i]);
	}
	printf("\n");
}

/* cria a hash dado um vetor de caracteres constante */
void hash(const char* s, unsigned char* dest){
	SHA256((const unsigned char*)s, strlen(s), dest);
}

/* cria uma segunda hash ao concatenar duas hashes */
void hashConcat(unsigned char d1[], unsigned char d2[], unsigned char dest[]){
	int i;
	unsigned char aux[SHA256_DIGEST_LENGTH * 2];

	for(i=0; i< SHA256_DIGEST_LENGTH; i++){
		aux[i] = d1[i];
	}
	for(;i< SHA256_DIGEST_LENGTH * 2;i++){
		aux[i] = d2[i - SHA256_DIGEST_LENGTH];
	}
	SHA256(aux,sizeof(aux),dest);
}

int comparaHash(unsigned char *d1, unsigned char *d2){
	if(*d1 == *d2)
		return 1;
	return 0;
}

ARV** criaArv(){
    ARV **nova;
    nova = (ARV**)malloc(sizeof(ARV*));

    if(nova != NULL){
        *nova = NULL;
    }

    return nova;
}

int existeArv(ARV **arv){
    if(arv == NULL){
        return 0;
    }
    return 1;
}

int vaziaArv(ARV **arv){
    if(!existeArv(arv)){
        return -1;
    }
    if(*arv == NULL){
        return 1;
    }
    return 0;
}

int procuranode(ARV *nodo,unsigned char *hash){
	
	if(nodo != NULL){
		if(comparaHash(nodo->info.hash,hash) == 1){
			return 1;
		}
		else{
			return procuranode(nodo->esq,hash) + procuranode(nodo->dir,hash);
		}
	}
	else{
		return 0;
	}

}

ARV* removeRaiz(ARV *nobase){
	ARV *aux1, *aux2, *arv = nobase;
	if(nobase == NULL){
		return NULL;
	}
	if(arv->esq == NULL){
		aux2 = arv->dir;
		free(arv);
		return aux2;
	}
	aux1 = arv;
	aux2 = arv->esq;
	while(aux2->dir != NULL){
		aux1 = aux2;
		aux2 = aux2->dir;
	}
	if(aux1 != arv){
		aux1->dir = aux2->esq;
		aux2->esq = arv->esq;
	}
	aux2->dir = arv->dir;
	free(arv);
	return aux2;
}

void destroiArv(ARV **arv){
	if(!existeArv(arv)){
		return;
	}
	while(!vaziaArv(arv)){
		*arv = removeRaiz(*arv);
	}
	free(arv);
	arv = NULL;
}

/* -------------------------------------------------- */

Blockchain **inicializa(){
	Blockchain **novo;
	novo = (Blockchain**)malloc(sizeof(Blockchain*));
	if(novo != NULL){
		*novo = NULL;
	}
	return novo;
}

void adiciona_blockchain(Blockchain **dados, Bloco *atual){
	Blockchain *novo;
	novo = (Blockchain*)malloc(sizeof(Blockchain));
	if(novo != NULL){
		if(*dados != NULL){
			novo->ant = *dados;
		}
		else{
			novo->ant = NULL;
		}
		novo->atual = atual;
		*dados = novo;
	}
}

/*Cria e inicializa o espaço para o bloco a ser gravado na blockchain */
Bloco* criar(){
	Bloco *novo;
	int i,j;
	novo = (Bloco*)malloc(sizeof(Bloco));
	if(novo == NULL){
		return NULL;
	}
	for(i=0;i<30;i++){
		(novo->timestamp)[i] = '\0';
	}
	for(i=0;i<32;i++){
		(novo->Hash)[i] = '\0';
		(novo->Hash_Pai)[i] = '\0';
	}
	for(i=0;i<16;i++){
		for(j=0;j<40;j++){
			((novo->lista)[i].Nome)[j] = '\0';	
		}
		for(j=0;j<40;j++){
			((novo->lista)[i].Diagnostico)[j] = '\0';	
		}
		for(j=0;j<40;j++){
			((novo->lista[i]).Medico)[j] = '\0';	
		}
		for(j=0;j<3;j++){
			((novo->lista[i]).idade)[j] = '\0';	
		}
	}
	return novo;
}

/* junta as informações de uma transação para serem usadas para criar a hash */
char *junta_data(Data info){
	char *data;
	data = (char*)calloc(120,sizeof(char));
	strcpy(data,info.Nome);
	strcat(data,info.idade);
	strcat(data,info.Diagnostico);
	strcat(data,info.Medico);
	return data;
}

/* Para quando os dados estão prontos e o bloco pode ser escrito */
void adiciona_bloco(Blockchain **Dados,Buffer buff){
	int i;
	time_t t;
	char *aux;
	Bloco *atual = criar();

	/* Alocando os 31 nós da árvore de hashes previamente, com 16 nós folhas */
	/* Diagrama para referência(indices no vetor):
									 30(hash do bloco)
					  28						      29
			   24	  		  25			   26			   27
		  16      17      18      19      20       21      22      23
		0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15
	*/
	ARV **no = (ARV**)malloc(sizeof(ARV*)*31);
	ARV** arvore = no;
	atual->arv = arvore;

	for(i=0;i<31;i++){
		no[i] = (ARV*)malloc(sizeof(ARV));
	}

	/* montando a arvore de baixo para cima */
	for(i=0;i<16;i++){
		(atual->lista)[i] = (buff.lista)[i];
		aux = junta_data((buff.lista)[i]);
		hash(aux,(no[i]->info).hash);
		free(aux);
		no[i]->dir = NULL;
		no[i]->esq = NULL;
	}

	for(i=0;i<15;i++){
		hashConcat((no[2*i]->info).hash, (no[2*i+1]->info).hash, (no[16+i]->info).hash);
	}

	for(i=0;i<15;i++){
		no[i+16]->dir = no[2*i+1];
		no[i+16]->esq = no[2*i];
	}
	no[30]->pai = NULL;
	*(atual->arv) = no[30];

	/* 'engravando' a hash do bloco em questão*/
	strcpy((char*)atual->Hash, (char*)(no[30]->info).hash);

	t = time(NULL);
	strcpy(atual->timestamp, ctime(&t));
	adiciona_blockchain(Dados, atual);
}

Blockchain* buscaBlockchain(Blockchain ** Dados, Data paciente){
	ARV *aux;
	Blockchain *block;
	char *aux2;
	int i = 0;
	unsigned char Hash[32];
	block = *Dados;

	while(block != NULL){

		aux = *(block->atual->arv);
		aux2 = junta_data(paciente);
		hash(aux2,Hash);
		i += procuranode(aux, Hash);
		free(aux2);
		
		if(i>0)
			break;

		block = block->ant;
	}
	return block;
}

/* busca paciente na blockchain pelo nome e imprime as instancias de mesmo nome*/
int buscaPaciente(Blockchain ** Dados, Data paciente){
	Blockchain *block = *Dados;
	int i = 0, j=0;

	while(block != NULL){
		for(i=0;i<16;i++){
			if(strcmp(((block->atual->lista)[i]).Nome, paciente.Nome) == 0){
				imprimirData((block->atual->lista)[i] ,0);
				j++;
			}
		}
		block = block->ant;
	}
	if(j == 0){
		return 0;
	}else{
		return 1;
	}
}

void imprimirData(Data paciente ,int i){
	printf("___________________________________________\n");
	printf("\tPaciente %d\n",i);
	printf("\tNome: %s\n",paciente.Nome);
	printf("\tIdade: %s\n",paciente.idade);
	printf("\tDiagnóstico: %s\n",paciente.Diagnostico);
	printf("\tMédico Responsável: %s\n",paciente.Medico);
	printf("___________________________________________\n\n" );
}

void imprimeBloco(Blockchain *block){
	int i,d=1;
	while(block != NULL){
		printf("\tBLOCO %d\n",d);
		printf("Timestamp: %s\n", block->atual->timestamp);
		printf("Hash: ");
		print(block->atual->Hash);
		d++;
		for(i=0;i<16;i++){
			imprimirData(block->atual->lista[i],i);
		}
		block = block->ant;
		printf("Digite enter para imprimir o proximo bloco(se houver): \n");
		getchar();
	}

}

/* Inicializa ou 'zera' as variaveis do buffer*/
void inicializaBuffer(Buffer *buff){
	int i,j;
	buff->contador = 0;
	for(i=0;i<16;i++){
		for(j=0;j<40;j++){
			((buff->lista)[i].Nome)[j] = '\0';	
		}
		for(j=0;j<40;j++){
			((buff->lista)[i].Diagnostico)[j] = '\0';	
		}
		for(j=0;j<40;j++){
			((buff->lista)[i].Medico)[j] = '\0';	
		}
		for(j=0;j<3;j++){
			((buff->lista)[i].idade)[j] = '\0';
		}
	}
}

/* Insere uma transação no buffer */
void encheBuffer(Buffer buff, Data paciente){
	int i;
	i = buff.contador;
	buff.lista[i] = paciente;
	buff.contador++;
}

/* Checa para ver se o buffer atingiu o máximo de transações */
int checaBuffer(Buffer buff){
	if(buff.contador == 16){
		return 1;
	}
	return 0;
}

/* NAO TAO INCOMPLETA */
void libera_blocos(Blockchain **Block){/*Nao teste ainda*/
	Blockchain *aux;
	while(*Block != NULL){
		aux = (*Block)->ant;
		destroiArv((*Block)->atual->arv);
		free((*Block)->atual);
		free(*Block);
		*Block = aux;
	}
	free(Block);
}