#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORTA 1789
void *funzione(void *arg);
struct mess{
int id;
char nome[50];
char dati[250];
};

//VARIABILI CONDIVISE//
pthread_mutex_t mutex;
struct mess messaggio;

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
//3 SETTO LA STRUCT //
saddr.sin_family=AF_INET;
saddr.sin_port=htons(PORTA);
saddr.sin_addr.s_addr=htonl(INADDR_ANY);
//4 FACCIO LA BIND//
if(bind(serversock,(struct sockaddr*)&saddr,sizeof(saddr))==-1){
perror("ERRORE DURANTE LA BIND\n");
exit(1);
}
//5 FACCIO LA LISTEN//
listen(serversock,5);
//6 AL INFINITO ACCETTO CONNESSIONI E CREO UN THREAD CHE ME LA GESTISCE//
while(1){
clientsock=accept(serversock,NULL,NULL);
if(clientsock==-1){
perror("ERRORE DURANTE L ACCEPT\n");
exit(1);
}
pthread_create(&t1,NULL,funzione,(void*)clientsock);
}
exit(0);
}

void *funzione(void *arg){
int sock=(int)arg;
int lun;
char buff[350];
char nome[50];
//CREO IL THREAD DETACH COSI DA NON FARE LA JOIN//
pthread_detach(pthread_self());
while(1){
lun=read(sock,buff,350);
if(lun<0){
perror("ERRORE DURANTE LA COMUNICAZIONE\n");
break;
}
if(strncmp(buff,"register",8)==0){
strcpy(nome,&buff[9]);
sprintf(buff,"WELCOME:%s\n",nome);
write(sock,buff,strlen(buff)+1);
}
if(strncmp(buff,"send",4)==0){
pthread_mutex_lock(&mutex);
messaggio.id++;
strcpy(messaggio.nome,nome);
strcpy(messaggio.dati,&buff[5]);
pthread_mutex_unlock(&mutex);
sprintf(buff,"OK");
write(sock,buff,strlen(buff)+1);
}
if(strncmp(buff,"read",4)==0){
pthread_mutex_lock(&mutex);
sprintf(buff,"%d %s %s\n",messaggio.id,messaggio.nome,messaggio.dati);
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

