#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <unistd.h>


#define REQ_QUEUE 	10010
#define RESP_QUEUE 	10020


#define MAX_TEXT_SIZE 	1000

//Struct para enviar
struct reqmsg {
 	long cli_id;
	char resposta_cli[MAX_TEXT_SIZE+1]; 
   	char comando[MAX_TEXT_SIZE+1];
	char path [MAX_TEXT_SIZE+1];
   	
	
};

//Struct para receber
struct respmsg {
	long cli_id;
	//char resposta_cli[MAX_TEXT_SIZE+1];
	char resposta[MAX_TEXT_SIZE+1];
};

void leArquivo()	
{
  FILE *arq;
  char Linha[100];
  char *result;
  int i;
  //system("clear");
  // Abre um arquivo TEXTO para LEITURA
  arq = fopen("log.txt", "rt");
  if (arq == NULL)  // Se houve erro na abertura
  {
     printf("Problemas na abertura do arquivo\n");
     return;
  }
  i = 1;
  while (!feof(arq))
  {
	// Lê uma linha (inclusive com o '\n')
      result = fgets(Linha, 500, arq);  // o 'fgets' lê até 99 caracteres ou até o '\n'
      //if (result)  // Se foi possível ler
	  printf( "%s",Linha);
      i++;
  }
  fclose(arq);  
}



void main()
{
    //Definição de variáveis
	int req_mq;
	int resp_mq;
	struct reqmsg cli_reqmsg;
	struct respmsg serv_respmsg;
	long cli_id;
    	int tempo;
	FILE *arq;
	char *path;

	system("clear");
	printf("================SHELL SISTOPERACIONAIS INICIOU===================\n");
	printf("=================================================================\n");			

//Define o id para conexão
	cli_id = getpid();
	req_mq = msgget(REQ_QUEUE, 0777);

//Verificação se vai falhar a conexão
	if (req_mq == -1) {
   		printf("msgget falhou no cliente\n"); 
		exit(1); 
	} 
	resp_mq = msgget(RESP_QUEUE, 0777);
	if (resp_mq == -1) {
   		printf("msgget falhou no cliente\n"); 
		exit(1); 
	} 
	do {
        //Limpa
		fflush(stdin);
		fflush(stdout);
		
		path = getcwd(path,0);       

        //Define a mensagem inicial a ser exibida
        printf("~ %s : ",path);
		scanf("%[^\n]s",cli_reqmsg.comando);
       	        scanf("%*c");			
		
		//strcpy(cli_reqmsg.path,path);
		
	         
		
			// Preenche o tipo da mensagem com o identificador (PID) do cliente
			cli_reqmsg.cli_id = cli_id;

			// Envia requisicao ao servidor
			msgsnd(req_mq,&cli_reqmsg,sizeof(struct reqmsg),0);
            
			// Espera pela mensagem de resposta especifica para este cliente
			// usando o PID do processo cliente como tipo de mensagem
			if (msgrcv(resp_mq,&serv_respmsg,sizeof(struct respmsg),cli_id,0) < 0){
				printf("msgrcv falhou no cliente\n");
				exit(1);
			}
			// Apresenta o nome da thread e que ela finalizou de rodar no servidor
			//printf("%s\n", serv_respmsg.resposta);
			
			leArquivo();
			
			path = getcwd(path,0);
			
			 
	} while(1);
			

	exit(0);	
}

