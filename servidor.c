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




#define REQ_QUEUE 	10010
#define RESP_QUEUE 	10020
#define MAX_TEXT_SIZE 	1000







//Struct para receber
struct reqmsg {
 	long cli_id;
	char resposta_cli[MAX_TEXT_SIZE+1];   	
    	char comando[MAX_TEXT_SIZE+1];
	char path [MAX_TEXT_SIZE+1];	
};
//Struct para enviar
struct respmsg {
	long cli_id;
	char resposta[MAX_TEXT_SIZE+1];
};



ls()
{

		  struct passwd *pw;
		  struct group *gp;
		  DIR *mydir;
		  char *c;
		  int i;
		  struct dirent *myfile;
		  struct stat fileStat;
		  mydir=opendir(".");
		  stat(".",&fileStat); 


 	 while((myfile=readdir(mydir))!=NULL)
 	{
		if(strcmp(myfile->d_name,".") != 0 && strcmp(myfile->d_name,"..") != 0)
		{
		    stat(myfile->d_name,&fileStat);  
		    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
		    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
		    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
		    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
		    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
		    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
		    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
		    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
		    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
		    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-"); 
		    printf(" ");
		    printf("%td ",fileStat.st_nlink);
		    pw=getpwuid(fileStat.st_uid);
		    printf("%s ",pw->pw_name);
		    gp=getgrgid(fileStat.st_gid);
		    printf("%s ",gp->gr_name);
		    printf("%td ",fileStat.st_size);
		    c=ctime(&fileStat.st_mtime);

		    for(i=4;i<=15;i++)
		      printf("%c",c[i]);

		    printf(" ");
		    printf("%s\n",myfile->d_name);
		}
	}

  closedir(mydir);  
  return 0;
}







//Função da thread, thread que executa o comando 
void *printMsg(struct reqmsg *cli_reqmsg) {

		//"ls -l /home/gui/Desktop>log.txt"
	
		if(strcmp(cli_reqmsg->comando,"dirlist")==0)
		{
			
			//if(fork()==0){execlp("/bin/sh", "sh", "-c","ls -l > log.txt ", cli_reqmsg->path, "log.txt", (char *)0);} 

			
			
 
		} 
		else if(strcmp(cli_reqmsg->comando,"myid")==0)
		{ 
			printf("MYID: %ld\n",cli_reqmsg->cli_id);
		} //mostra id do cliente			
		else if(strcmp(cli_reqmsg->comando,"exit")==0)
		{ 
			exit(0);
		}
	   	
	strcpy(cli_reqmsg->resposta_cli,"resposta");
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

		// Envia a resposta ao cliente
		msgsnd(resp_mq,&serv_respmsg,sizeof(struct respmsg),0);
		printf("servidor: enviou resposta ao cliente %ld\n",serv_respmsg.cli_id);
	}

	exit(0);	
}

