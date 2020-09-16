#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define PORTA 1789
//VARIABILI GLOBALI//
int sock;
void *worker_thread(void *args);

int main(void){
//MI SERVE LA STRUCT DEGLI INDIRIZZI,UN THREAD E UN BUFF//
struct sockaddr_in saddr;
pthread_t t1;
char buff[300];

//STEP 1 CREO IL SOCKET//	
sock=socket(AF_INET,SOCK_STREAM,0);
if(sock==-1){
printf("SOCKET NON CREATO CORRETTAMENTE\n");
exit(1);
}
//STEP 2 SETTO LA STRUCT DEGLI INDIRIZZI//
saddr.sin_family=AF_INET;
saddr.sin_port=htons(PORTA);
saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
//STEP 3 FACCIO LA CONNECT//
if(connect(sock, (struct sockaddr *)&saddr, sizeof(saddr))==-1){
		perror("Errore connect");
		exit(1);
	}
//PASSO 4 CREO UN THREAD/
pthread_create(&t1,NULL,worker_thread,NULL);
//PASSO 5 AL INFINITO PERMETTO DI INSERIRE UN COMANDO//
while(1){
                printf("Inserisci un comando: ");
		fgets(buff, 250, stdin);
		buff[strlen(buff)-1]='\0';
		write(sock, buff, strlen(buff)+1);//SCRIVO SULLA SOCKET IL COMANDO INSERITO//
		if(strncmp(buff, "close", 5)==0){
			break;
		}
	}
	//FACCIO LA JOIN FACCIO LA CLOSE ED HO FINITO//
	pthread_join(t1,NULL);//FACCIO LA JOIN//
	close(sock);//FACCIO LA CLOSE//
	printf("TUTTO E ANDATO CORRETTAMENTE\n");
	exit(0);
}


void *worker_thread(void *args){
char buff[300];
int lun;
//QUESTI 2 SERVONO PER NON SCIARIARSI//
int id=0;
int rid=0;
while(1){
                sprintf(buff,"read");//METTO NEL BUFF READ//
		write(sock, buff, strlen(buff)+1);//SCRIVO NELLA SOCKET IL COMANDO READ//
		lun=read(sock,buff,500);//LEGGO DAL SOCKET QUELLO CHE IL SERVER MI FORNISCE IN CORRISPONDENZA DI TALE COMANDO//
                if(lun<=0){
                  printf("ERRORE DURANTE LA COMUNICAZIONE\n");
                  break;
               }
                if(strncmp(buff, "ok", 2)==0){//LA SEND E  FATTA CORRETTAMENTE
				printf("Ricevuto: %s\n",buff); //VISUALIZZO QUELLO CHE HO NEL BUFF//
				printf("Inserisci comando: "); //COSI NON PERDO QUESTA VISUALIZZAZIONE//
			}
			
				sscanf(buff,"%d %*s",&id);//PRELEVO L'ID DEL MESSAGGIO DAL BUFFER//
				if(rid!=id){
					printf("Ricevuto: %s\n",buff);//VISUALIZZO QUELLO CHE HO NEL BUFF//
					printf("Inserisci comando: ");
					rid=id;//COSI LO STAMPO UNA SOLA VOLTA//
				}
      sleep(1);
	}
      pthread_exit(0);
}
