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
	long cod_mansgem;
};

//Struct para receber
struct respmsg {
	long cli_id;
	//char resposta_cli[MAX_TEXT_SIZE+1];
	char resposta[MAX_TEXT_SIZE+1];
};

long int id_usuario(char init[8]){
char atual;
long count=0;
int i = 0;

for (i; i < strlen(init); i++)
{if (init[i]=='a'){count = count + 250;}else if (init[i]=='b'){count = count + 575;}else if (init[i]=='c'){count = count + 20;}else if (init[i]=='d')
{count = count + 95;}else if (init[i]=='e'){count = count + 43;}else if (init[i]=='f'){count = count + 81;}else if (init[i]=='g'){count = count + 888;}
else if (init[i]=='h'){count = count + 5;}else if (init[i]=='i'){count = count + 34;}else if (init[i]=='j'){count = count + 475;}else if (init[i]=='k'){
count = count + 121;}else if (init[i]=='l'){count = count + 1221;}else if (init[i]=='m'){count = count + 632;}else if (init[i]=='n'){count = count + 864;
}else if (init[i]=='o'){count = count + 999;}else if (init[i]=='p'){count = count + 666;}else if (init[i]=='q'){count = count + 4123;}else if (init[i]=='r'){
count = count + 71;}else if (init[i]=='s'){count = count + 562;}else if (init[i]=='t'){count = count + 324;}else if (init[i]=='u'){count = count + 101;
}else if (init[i]=='v'){count = count + 21;}else if (init[i]=='x'){count = count + 17;}else if (init[i]=='z'){count = count + 13;}else if (init[i]=='0'){
count = count + 2019;}else if (init[i]=='1'){count = count + 45;}else if (init[i]=='2'){count = count + 12;}else if (init[i]=='3'){count = count + 751;
}else if (init[i]=='4'){count = count + 2101;}else if (init[i]=='5'){count = count + 1111;}else if (init[i]=='6'){count = count + 777;}else if (init[i]=='7')
{count = count + 642;}else if (init[i]=='8'){count = count + 314;}else if (init[i]=='9'){count = count + 210;}else{count = count +0;}
}
return count;

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
	char *path;
	char init[8];

	system("clear");
	printf("=================================================================\n");	
	printf("================SHELL SISTOPERACIONAIS INICIOU===================\n");
	printf("===============DIGITE HELP PARA VER OS COMANDOS==================\n");
	printf("============APERTE ENTER PARA EXECUTAR OS COMANADOS==============\n");	
	printf("=================================================================\n");			
/*
	while(1){
	printf("digite o Login: ");
		scanf("%s", init);
       scanf("%*c");
	cli_reqmsg.cli_id = id_usuario(init);
	//cli_reqmsg.cli_id = cli_id;
	printf("%ld\n", cli_reqmsg.cli_id);
	if (cli_reqmsg.cli_id=!0){break;}
	printf("Login invalido\n");}
	//cli_reqmsg.cli_id = cli_id;*/

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
	/*
	printf("%ld", cli_reqmsg.cli_id);
	printf("%ld", cli_id);
	
	strcpy(cli_reqmsg.comando,"login");
	msgsnd(req_mq,&cli_reqmsg,sizeof(struct reqmsg),0);
	if (msgrcv(resp_mq,&serv_respmsg,sizeof(struct respmsg),cli_id,0) < 0){
	printf("msgrcv falhou no cliente\n");
	exit(1);}
	printf("%s\n", serv_respmsg.resposta);*/

	do {


        //Limpa
		fflush(stdin);
		fflush(stdout);
		
		//copia o path
		
		path = getcwd(path,0);  
		strcpy(cli_reqmsg.path,path);     
		
        //Define a mensagem inicial a ser exibida
        printf("~ %s: Entre com o Comando: ",path);
		scanf("%[^\n]s",cli_reqmsg.comando);
       	scanf("%*c");
		
		if(strcmp(cli_reqmsg.comando,"godir")==0||(cli_reqmsg.comando,"GODIR")==0){printf("Digite o caminho \n");scanf("%[^\n]s",cli_reqmsg.caminho);scanf("%*c");chdir(cli_reqmsg.caminho);}			
		if(strcmp(cli_reqmsg.comando,"send")==0||(cli_reqmsg.comando,"SEND")==0||strcmp(cli_reqmsg.comando,"run")==0||(cli_reqmsg.comando,"RUN")==0){
			 printf("~ %s : Digite sua mensagem: ",path);
				scanf("%[^\n]s",cli_reqmsg.texto_msg);
       	        scanf("%*c");
		if(strcmp(cli_reqmsg.comando,"send")==0||(cli_reqmsg.comando,"SEND")==0){
			printf("~ %s : Digite o id destino: ",path);
				scanf("%ld",&cli_reqmsg.id_destino);
       	        scanf("%*c");}
		}
		if(strcmp(cli_reqmsg.comando,"showmail")==0||(cli_reqmsg.comando,"SHOWMAIL")==0){
		do{msgsnd(req_mq,&cli_reqmsg,sizeof(struct reqmsg),0);if (msgrcv(resp_mq,&serv_respmsg,sizeof(struct respmsg),cli_id,0) < 0){
		printf("msgrcv falhou no cliente\n");exit(1);}printf("%s\n", serv_respmsg.resposta);}while(strcmp(serv_respmsg.resposta, "")!=0);
				
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
