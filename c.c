//SERVER CHE UTILIZZA LE SOCKET NEL DOMINIO DI INTERNET,CHE PERMETTE A DEI CLIENT DI INSERIRE DEI COMANDI ED IN BASE AI COMANDI RICEVUTI FA QUALCOSA//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#define PORTA 1789
void *funzione(void *arg);
struct msgbuf{
int id;
char nome[50];
char mess[200];
};
//VARIABILI CONDIVISE//
pthread_mutex_t mutex;
struct msgbuf messaggio;

int main(void){
int serversock;
int clientsock;
pthread_t t1;
struct sockaddr_in saddr;

//1 INIZIALIZZO IL MUTEX//
pthread_mutex_init(&mutex,NULL);

//2 CREO IL SOCKET//
serversock=socket(AF_INET,SOCK_STREAM,0);
if(serversock==-1){
perror("ERRORE DURANTE LA CREAZIONE DEL SOCKET\n");
exit(1);
}
//3 SETTO LA STRUCT DEGLI INDIRIZZI//
saddr.sin_family=AF_INET;
saddr.sin_port=htons(PORTA);
saddr.sin_addr.s_addr=htonl(INADDR_ANY);
bzero(&messaggio,sizeof(messaggio));
//4 FACCIO LA BIND//
if(bind(serversock,(struct sockaddr*)&saddr,sizeof(saddr))==-1){
perror("ERRORE DURANTE LA BIND\n");
exit(1);
}
//5 FACCIO LA LISTEN//
listen(serversock,5);
//6 AL INFINITO ACCETTO CONNESSIONI E CREO UN THREAD CHE MI GESTISCE OGNI CONNESSIONE//
while(1){
clientsock=accept(serversock,NULL,NULL);
if(clientsock==-1){
perror("ERRORE DURANTE LA BIND\n");
exit(1);
}
pthread_create(&t1,NULL,funzione,(void*)clientsock);
}
exit(0);
}



void *funzione(void *arg){
int lun=0;
int sock=(int)arg;
char buff[300];
char nome[50];
//FACCIO IL THREAD DETACH//
pthread_detach(pthread_self());

while(1){
lun=read(sock,buff,300);
if(lun<0){
perror("ERRORE DURANTE LA COMUNICAZIONE\n");
exit(1);
}
if(strncmp(buff,"registrati",10)==0){
strcpy(nome,&buff[11]);
sprintf(buff,"BENVENUTO:%s\n",nome);
write(sock,buff,strlen(buff)+1);
}
if(strncmp(buff,"send",4)==0){
pthread_mutex_lock(&mutex);
messaggio.id++;
strcpy(messaggio.nome,nome);
strcpy(messaggio.mess,&buff[5]);
pthread_mutex_unlock(&mutex);
sprintf(buff,"OK");
write(sock,buff,sizeof(buff)+1);
}
if(strncmp(buff,"leggi",5)==0){
pthread_mutex_lock(&mutex);
sprintf(buff,"%d %s %s",messaggio.id,messaggio.nome,messaggio.mess);
pthread_mutex_unlock(&mutex);
write(sock,buff,strlen(buff)+1);
}
if(strncmp(buff,"close",5)==0){
break;
}
}
close(sock);
pthread_exit(0);
}
