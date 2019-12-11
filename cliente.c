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
	char caminho [MAX_TEXT_SIZE+1];
	char texto_msg[MAX_TEXT_SIZE+1];
	long id_destino;
   	
	
};

//Struct para receber
struct respmsg {
	long cli_id;
	//char resposta_cli[MAX_TEXT_SIZE+1];
	char resposta[MAX_TEXT_SIZE+1];
};


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
	printf("=================================================================\n");	
	printf("================SHELL SISTOPERACIONAIS INICIOU===================\n");
	printf("===============DIGITE HELP PARA VER OS COMANDOS==================\n");
	printf("============APERTE ENTER PARA EXECUTAR OS COMANADOS==============\n");	
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
		
		//copia o path
		path = getcwd(path,0);  
		strcpy(cli_reqmsg.path,path);     

        //Define a mensagem inicial a ser exibida
        printf("~ %s : ",path);
		scanf("%[^\n]s",cli_reqmsg.comando);
       	scanf("%*c");

		//verifica para mudar o diretório	
		if(strcmp(cli_reqmsg.comando,"godir")==0||(cli_reqmsg.comando,"GODIR")==0){
			printf("Digite o caminho \n");scanf("%[^\n]s",cli_reqmsg.caminho);scanf("%*c");chdir(cli_reqmsg.caminho);	
		}		
		if(strcmp(cli_reqmsg.comando,"logar")==0||(cli_reqmsg.comando,"LOGAR")==0){
			 printf("~ %s : Digite seu id ",path);
				scanf("%ld", &cli_id);
       	        scanf("%*c");	
		}	
		if(strcmp(cli_reqmsg.comando,"send")==0||(cli_reqmsg.comando,"SEND")==0||strcmp(cli_reqmsg.comando,"run")==0||(cli_reqmsg.comando,"RUN")==0){
			 printf("~ %s : Digite sua mensagem: ",path);
				scanf("%[^\n]s",cli_reqmsg.texto_msg);
       	        scanf("%*c");
		if(strcmp(cli_reqmsg.comando,"send")==0||(cli_reqmsg.comando,"SEND")==0){
			printf("~ %s : Digite o id destino: ",path);
				scanf("%ld",&cli_reqmsg.id_destino);
       	        scanf("%*c");}
		}
		//copia o id
			cli_reqmsg.cli_id = cli_id;

		// Envia requisicao ao servidor
			msgsnd(req_mq,&cli_reqmsg,sizeof(struct reqmsg),0);
            
		// Espera pela mensagem de resposta especifica para este cliente
		// usando o PID do processo cliente como tipo de mensagem
			
		if (msgrcv(resp_mq,&serv_respmsg,sizeof(struct respmsg),cli_id,0) < 0){
			printf("msgrcv falhou no cliente\n");
			exit(1);}
				
		//Apresenta resposta do servidor
		printf("%s\n", serv_respmsg.resposta);
			
		//copia o path
		path = getcwd(path,0);
		
			 
	} while(1);
			

	exit(0);	
}

