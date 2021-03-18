#include "auxiliar.h"

int main(){
	Buffer buff;
	int op =0;
	Data aux;
	Blockchain **Dados = inicializa();
	inicializaBuffer(&buff);	
	do{
		printf("		MENU\n");
		printf("___________________________________________ \n");
		printf("											\n");
		printf("(1) Registrar dados do paciente			\n");
		printf("(2) Imprime instâncias de um paciente 		\n");
		printf("(3) Imprimir todos os blocos	 			\n");
		printf("(0) Sair do programa						\n");
		printf("___________________________________________\n\n" );
		scanf("%d",&op);
		switch(op){
			case 1:
				while(getchar() != '\n');
				printf("Digite o nome do paciente\n");
				scanf("%[^\n]",((buff.lista)[buff.contador]).Nome);
				while(getchar() != '\n');
				printf("Digite a idade do paciente\n");
				scanf("%[^\n]",((buff.lista)[buff.contador]).idade);
				while(getchar() != '\n');
				printf("Digite o diagnóstico do paciente\n");
				scanf("%[^\n]",((buff.lista)[buff.contador]).Diagnostico);
				while(getchar() != '\n');
				printf("Digite o nome do Médico responsável\n");
				scanf("%[^\n]",((buff.lista)[buff.contador]).Medico);
				if(buscaBlockchain(Dados,buff.lista[buff.contador]) == NULL){
					buff.contador++;
					printf("paciente inserido:%s\n",buff.lista[buff.contador -1].Nome);
				}else{
					printf("Paciente já cadastrado\n");
				}
				if(checaBuffer(buff)){
					printf("-------------------------------------");
					printf("\n %% Bloco escrito no sistema! %% \n");
					printf("-------------------------------------\n");
					adiciona_bloco(Dados, buff);
					inicializaBuffer(&buff);
				}
				printf("\n\n");
				break;
			case 2:
				printf("Digite o nome do paciente: ");
				while(getchar() != '\n');
				scanf("%[^\n]",aux.Nome);
				if(buscaPaciente(Dados,aux)){
				}else{
					printf("Paciente não cadastrado\n");		
				}
				break;
			case 3:
				imprimeBloco(*Dados);
				break;
			default:
				continue;
		}
	}while(op != 0);

	libera_blocos(Dados);
	return 0;
}
