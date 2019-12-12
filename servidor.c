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
#include <signal.h> 




#define REQ_QUEUE 	10010
#define RESP_QUEUE 	10020
#define MAX_TEXT_SIZE 	1000




char concat[MAX_TEXT_SIZE+1];


//Struct para receber
struct reqmsg {
 	long cli_id;
	long climensagem_id;
	char resposta_cli[MAX_TEXT_SIZE+1];   	
    	char comando[MAX_TEXT_SIZE+1];
	char mensagem_origem[MAX_TEXT_SIZE+1];
	char path [MAX_TEXT_SIZE+1];	
};
//Struct para enviar
struct respmsg {
	long cli_id;
	char resposta[MAX_TEXT_SIZE+1];
	char mensagem_destino[MAX_TEXT_SIZE+1];
	
};



char * ls(char path[100])
{

		  memset (&concat, 0, sizeof (concat) );
			
		  struct passwd *pw;
		  struct group *gp;
		  DIR *mydir;
		  char *c;		  
		  int i,linhaLs;
		  struct dirent *myfile;
		  struct stat fileStat;
		  mydir=opendir(path);
		  stat(".",&fileStat);
		  	

		  

 	 while((myfile=readdir(mydir))!=NULL)
 	{
		char 		stat1[MAX_TEXT_SIZE+1],stat2[MAX_TEXT_SIZE+1],stat3[MAX_TEXT_SIZE+1],stat4[MAX_TEXT_SIZE+1],stat5[MAX_TEXT_SIZE+1],stat6[MAX_TEXT_SIZE+1],stat7[MAX_TEXT_SIZE+1],stat8[MAX_TEXT_SIZE+1],stat9[MAX_TEXT_SIZE+1],stat10[MAX_TEXT_SIZE+1],stat11[MAX_TEXT_SIZE+1];
		   
		
		
			if(strcmp(myfile->d_name,".") != 0 && strcmp(myfile->d_name,"..") != 0)
			{
 			  
			    //pegando variaveis 
			     stat(myfile->d_name,&fileStat);  
			     strcpy(stat1,(S_ISDIR(fileStat.st_mode)) ? "d" : "-");
			     strcpy(stat2,(fileStat.st_mode & S_IRUSR) ? "r" : "-");		    
			     strcpy(stat3,(fileStat.st_mode & S_IWUSR) ? "w" : "-");
			     strcpy(stat4,(fileStat.st_mode & S_IXUSR) ? "x" : "-");
			     strcpy(stat5,(fileStat.st_mode & S_IRGRP) ? "r" : "-");
			     strcpy(stat6,(fileStat.st_mode & S_IWGRP) ? "w" : "-");
			     strcpy(stat7,(fileStat.st_mode & S_IXGRP) ? "x" : "-");
			     strcpy(stat8,(fileStat.st_mode & S_IROTH) ? "r" : "-");
			     strcpy(stat9,(fileStat.st_mode & S_IWOTH) ? "w" : "-");
			     strcpy(stat10,(fileStat.st_mode & S_IXOTH) ? "x" : "-"); 
			    
			    	//strcpy(stat11,fileStat.st_nlink);
			    	
				pw=getpwuid(fileStat.st_uid);
			    
			    	gp=getgrgid(fileStat.st_gid);			   
			   
			    	//c=ctime(&fileStat.st_mtime);
			    	
			    	strcat(concat,stat1);
				strcat(concat,stat2);
				strcat(concat,stat3);
				strcat(concat,stat4);
				strcat(concat,stat5);
				strcat(concat,stat6);
				strcat(concat,stat7);
				strcat(concat,stat8);
				strcat(concat,stat9);
				strcat(concat,stat10);
				strcat(concat," ");
				strcat(concat,gp->gr_name);
				strcat(concat," ");
				//for(i=4;i<=15;i++)
				//{
					//char str[10];
					//sprintf(str, "%i", c[i]);
				      	//strcat(concat,str);
				//}
				strcat(concat,pw->pw_name);
				strcat(concat," ");
				strcat(concat,myfile->d_name);				
				strcat(concat,"\n");
				
				//strcat(concat,stat11);
				//strcat(concat,pw);
				//strcat(concat,gp);
				//strcat(concat,c);		
			   
			}		
		
	}

	

  closedir(mydir);  
  return concat;
}







//Função da thread, thread que executa o comando 
void *printMsg(struct reqmsg *cli_reqmsg) {

		
		
		if(strcmp(cli_reqmsg->comando,"dirlist")==0)
		{		
			char *resposta_ls = ls(cli_reqmsg->path);
			
			strcpy(cli_reqmsg->resposta_cli,resposta_ls); 
		} 
		else if(strcmp(cli_reqmsg->comando,"myid")==0)
		{
			char id[10];
						 
			//função para converter o id em char para passar como resposta			
			snprintf(id, 6, "%ld", cli_reqmsg->cli_id);
			
			strcpy(cli_reqmsg->resposta_cli,id);
		}
		else if(strcmp(cli_reqmsg->comando,"send")==0)
		{			 
				

		} 			
		else if(strcmp(cli_reqmsg->comando,"exit")==0)
		{
			//mata o processo do cliente do id enviado 
			kill(cli_reqmsg->cli_id, SIGINT);
		}
		else// caso o comando nao exista ele diz que nao encontrou o comando
		{
			strcpy(cli_reqmsg->resposta_cli,"Comando não encontrado");
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
			printf("criando nova thread %s \n", cli_reqmsg.comando);
			pthread_t thread;
			pthread_create(&thread, NULL, (void *)printMsg, &cli_reqmsg);
			pthread_join(thread, NULL);	


		// Copia o identificador do cliente para a mensagem de resposta
		serv_respmsg.cli_id = cli_reqmsg.cli_id;

		// Copia o nome para a mensagem de resposta
		strcpy(serv_respmsg.resposta,cli_reqmsg.resposta_cli);

		("mensagem: %s /// id de destino: %ld ", cli_reqmsg.mensagem_origem, cli_reqmsg.climensagem_id);	


		// Envia a resposta ao cliente
		msgsnd(resp_mq,&serv_respmsg,sizeof(struct respmsg),0);
		printf("servidor: enviou resposta ao cliente %ld\n",serv_respmsg.cli_id);
	}

	exit(0);	
}

