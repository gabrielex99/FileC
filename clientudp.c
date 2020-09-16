//MANDIAMO COMANDI AL DRONE
//IL CLIENT DEVE PERMETTERE DI MANDARE DEI MESSAGGI AL SERVER,ABBIAMO 2 THREAD MAIN THREAD INVIA LE STRINGHE,ALTRO THREAD RX,STA IN ASCOLTO COSI DA STAMPARE LE RISPOSTE DEL SERVER//
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define STRUDIM 50
#define STRMDIM 140
#define PORTA 8889
//NON HO MUTEX PERCHE NON HO ROBE CONDIVISE//
void *worker_thread(void *args);
int sock;

int main(){
	pthread_t wtid;
        char buff[250];
        struct sockaddr_in saddr;
        //CREO IL SOCKET//
	sock=socket(AF_INET, SOCK_DGRAM, 0);//CREA IL SOCKET//
	if(sock==-1){
         perror("ERRORE NELLA CREAZIONE DEI SOCKET\n");
        }
        //2 SETTO LA STRUCT DEGLI INDIRIZZI//
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(PORTA);//CONVERSIONE HOST TO NETWORK SHORT//
	saddr.sin_addr.s_addr=inet_addr("192.168.10.1");//ACCETTO LA CONNESSIONE SOLO DA QTALE IP//
        //3 FACCIO LA BIND//
	if(bind(sock,(struct sockadr*)&saddr,sizeof(saddr))<0){
        perror("ERRORE BIND!\n");
        exit(1);
} 
      //4 CREO UN THREAD//   
      pthread_create(&wtid, NULL, worker_thread,NULL);//CREO IL THREAD//
      while(1){
      printf("Inserisci comando\n");
      fgets(buff,250,stdin);
      buff[strlen(buff)-1]='\0'; //TOGLIAMO IL CARATTERE DI RITORNO LETTO DA FGETS//
      send_to(sock,buff,strlen(buff),0,(struct sockaddr*)&saddr,sizeof(saddr));//MANDIAMO LA STRINGA AL SERVER//
      if(strncmp(buff,"close",5)==0){
      break;
}
       }
pthread_join(wtid,NULL);
close(sock);
printf("HO FINITO\n");
}

void* worker_thread(void *args){
char buff[500];
int len;
while(1){
len=recvfrom(sock,buff,500,0,NULL,NULL);
if(len>0){
printf("\n %s\n Inserisco comando:",buff);
}
}
}
