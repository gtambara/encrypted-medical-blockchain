#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <time.h>

/* TAMANHO DA HASH ( SHA256_DIGEST_LENGTH ) = 32 */

/* dadosA : dados Arvore */
struct dadosa{
    unsigned char hash[32];
};
typedef struct dadosa Datab;

struct node{
    Datab info;
    struct node *esq;               /*no filho esquerdo*/
    struct node *dir;               /*no filho direito*/
    struct node *pai;               /*no pai*/
};
typedef struct node ARV;

/* dados da transação */
struct data{
	char Nome[40];
	char idade[3];
	char Diagnostico[40];
	char Medico[40];
};
typedef struct data Data;

struct bloco{
	unsigned char Hash[32];
	unsigned char Hash_Pai[32];
	ARV **arv;
	Data lista[16];
	char timestamp[30];
};
typedef struct bloco Bloco;

struct Blc{
	Bloco *atual;
	struct Blc *ant;
};
typedef struct Blc Blockchain;

struct buffer{
	Data lista[16];
	int contador;
};
typedef struct buffer Buffer;

/* FUNCOES DE HASH */
void print(unsigned char *hashf);
void hash(const char *s, unsigned char *dest);
void hashConcat(unsigned char d1[], unsigned char d2[], unsigned char dest[]);
int comparaHash(unsigned char *d1, unsigned char *d2);

/* FUNCOES DA ARVORE */
ARV** criaArv();
int existeArv(ARV **arv);
int vaziaArv(ARV **arv);
ARV* removeRaiz(ARV *nobase);
void destroiArv(ARV **arv);
void caminhaComparando(ARV *arv, unsigned char *hash, int *a);
int procuranode(ARV *nodo,unsigned char *hash);

/* FUNCOES DA BLOCKCHAIN */
Blockchain **inicializa();
void adiciona_blockchain(Blockchain **dados, Bloco *atual);
Bloco* criar();
char* junta_data(Data info);
void adiciona_bloco(Blockchain **Dados,Buffer buff);
void inicializaBuffer(Buffer *buff);
void encheBuffer(Buffer buff, Data paciente);
int checaBuffer(Buffer buff);
void libera_blocos(Blockchain **Block);
Blockchain* buscaBlockchain(Blockchain ** Dados, Data paciente);
void imprimirData(Data paciente, int i);
void imprimeBloco(Blockchain *block);
int buscaPaciente(Blockchain ** Dados, Data paciente);