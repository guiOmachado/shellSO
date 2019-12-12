#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/socket.h>

#define _XOPEN_SOURCE_EXTENDED 1
#define REQ_QUEUE 	10010
#define RESP_QUEUE 	10020
#define MAX_TEXT_SIZE 	1000

//Struct para receber
struct reqmsg {
 	long cli_id;
	char resposta_cli[MAX_TEXT_SIZE+1];   
	char comando[MAX_TEXT_SIZE+1];
	char path [MAX_TEXT_SIZE+1];	
	char caminho [MAX_TEXT_SIZE+1];
	char texto_msg[MAX_TEXT_SIZE+1];	
	long id_destino;
	char aux[MAX_TEXT_SIZE+1];
};
//Struct para enviar
struct respmsg {
	long cli_id;
	char resposta[MAX_TEXT_SIZE+1];
};


//Função da thread, thread que executa o comando 
void *printMsg(struct reqmsg *cli_reqmsg) {

		
		if(strcmp(cli_reqmsg->comando,"dirlist")==0||strcmp(cli_reqmsg->comando,"DIRLIST")==0)
		{
			
			
		} else if (strcmp(cli_reqmsg->comando,"clear")==0|| strcmp(cli_reqmsg->comando,"CLEAR")==0)
		{
			strcpy(cli_reqmsg->resposta_cli, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n=================================================================\n================SHELL SISTOPERACIONAIS INICIOU===================\n===============DIGITE HELP PARA VER OS COMANDOS==================\n============APERTE ENTER PARA EXECUTAR OS COMANADOS==============\n=================================================================\n");
		}
		else if(strcmp(cli_reqmsg->comando,"myid")==0|| strcmp(cli_reqmsg->comando,"MYID")==0)
		{ 
			long x = cli_reqmsg->cli_id;
			sprintf(cli_reqmsg->resposta_cli, "\n~ %s : Seu id é: %ld", cli_reqmsg->path, x);
			
		} //mostra id do cliente
		else if(strcmp(cli_reqmsg->comando,"godir")==0||strcmp(cli_reqmsg->comando,"GODIR")==0)
		{ 
			
		}			
		else if(strcmp(cli_reqmsg->comando,"exit")==0 || strcmp(cli_reqmsg->comando,"EXIT")==0)
		{ 
			exit(0);
		}
		else if(strcmp(cli_reqmsg->comando,"help")==0||strcmp(cli_reqmsg->comando,"HELP")==0)
		{ 
			strcpy(cli_reqmsg->resposta_cli,"DIRLIST: Lista os diretórios\nMYID: Mostra seu ID\nGODIR: Vai para o diretório desejado\nRUN: Roda a aplicação\nEXIT: Faz o logoff\nMAIL: Envia a mensagem de e-mail\nSHOWMAIL: Mostra todas as mensagens de e-mails recebidas\nSEND: Envia uma mensagem direta ao usuário destino\nRECEIVE: Retira e mostra na tela uma mensagem (se houver) da fila de mensagens\nUSERS: Mostra na tela a lista de usuários correntemente logados no sistema\nCLEAR: Limpa a tela\n");
		}
		else if(strcmp(cli_reqmsg->comando,"run")==0||strcmp(cli_reqmsg->comando,"RUN")==0)
		{ 
			
			printf("\n\n%s\n\n", cli_reqmsg->aux);
			long x = cli_reqmsg->cli_id;
			sprintf(cli_reqmsg->resposta_cli, " gnome-terminal -e %s", cli_reqmsg->texto_msg);
			system(cli_reqmsg->resposta_cli);
			printf("\n\n%s\n\n", cli_reqmsg->resposta_cli);
			
		}
		else if(strcmp(cli_reqmsg->comando,"mail")==0||strcmp(cli_reqmsg->comando,"MAIL")==0)
		{ 	
		
		}
		else if(strcmp(cli_reqmsg->comando,"showmail")==0||strcmp(cli_reqmsg->comando,"SHOWMAIL")==0)
		{ 
			
		}
		else if(strcmp(cli_reqmsg->comando,"send")==0||strcmp(cli_reqmsg->comando,"SEND")==0)
		{ 
			strcpy(cli_reqmsg->resposta_cli,"Mensagem enviada!!\n");

		}
		else if(strcmp(cli_reqmsg->comando,"receive")==0||strcmp(cli_reqmsg->comando,"RECEIVE")==0)
		{ 
			
		}
		else if(strcmp(cli_reqmsg->comando,"users")==0||strcmp(cli_reqmsg->comando,"USERS")==0)
		{ 
			
		}else{
			sprintf(cli_reqmsg->resposta_cli,"~ %s : Comando não encontrado!!\n", cli_reqmsg->path);
		}
	   	
	
}


void main()
{
	//Definição de variáveis
	int req_mq;
	int resp_mq;
	struct reqmsg cli_reqmsg;
	struct respmsg serv_respmsg;
	
	//Verificação se vai falhar a conexão
	req_mq = msgget(REQ_QUEUE, IPC_CREAT | 0777);
	if (req_mq == -1) {
   		printf("msgget falhou no servidor\n"); 
		exit(1); 
	} 
	resp_mq = msgget(RESP_QUEUE, IPC_CREAT | 0777);
	if (resp_mq == -1) {
   		printf("msgget falhou no servidor\n"); 
		exit(1); 
	} 
	printf("servidor: iniciou execucao\n");
	for (;;) {
		// Espera pela requisicao de qualquer cliente (message type=0)
		if (msgrcv(req_mq,&cli_reqmsg,sizeof(struct reqmsg),0,0)<0) {
			printf("msgrcv falhou no servidor\n");
			exit(1);
		}

		printf("servidor: recebeu requisicao do cliente %ld\n",cli_reqmsg.cli_id);
		
		// Cria a thread requerida
			pthread_t thread;
			pthread_create(&thread, NULL, (void *)printMsg, &cli_reqmsg);
			pthread_join(thread, NULL);	

		// Copia o identificador do cliente para a mensagem de resposta
		serv_respmsg.cli_id = cli_reqmsg.cli_id;

		
		// Copia o nome para a mensagem de resposta
		strcpy(serv_respmsg.resposta,cli_reqmsg.resposta_cli);
		// Envia a resposta ao cliente
		msgsnd(resp_mq,&serv_respmsg,sizeof(struct respmsg),0);
		printf("servidor: enviou resposta ao cliente %ld\n",serv_respmsg.cli_id);

	if (strcmp(cli_reqmsg.comando,"send")==0||(cli_reqmsg.comando,"SEND")==0)
	{
		serv_respmsg.cli_id = cli_reqmsg.id_destino;
		strcpy(serv_respmsg.resposta,cli_reqmsg.texto_msg);
		msgsnd(resp_mq,&serv_respmsg,sizeof(struct respmsg),0);
	}

		

	}

	exit(0);	
}

