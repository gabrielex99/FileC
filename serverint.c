#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include<arpa/inet.h>
#define PORTA 1789
void *funzione(void *arg);
struct mess{
int id;
char nome[50];
int valore;
};
//VARIABILI CONDIVISI//
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
//3 setto la struct degli indirizzi//
saddr.sin_family=AF_INET;
saddr.sin_port=htons(PORTA);
saddr.sin_addr.s_addr=htonl(INADDR_ANY);
//4 FACCIO LA BIND//
if(bind(serversock,(struct sockaddr*)&saddr,sizeof(saddr))==-1){
perror("ERRORE DURANTE LA BIND\n");
exit(1);
}
//5 faccio la listen//
listen(serversock,5);
//AL INFINITO ACCETTO CONNESSIONI E CREO IL THREAD//
while(1){
clientsock=accept(serversock,NULL,NULL);
if(clientsock==-1){
perror("ERRORE DURANTE L'ACCEPT\n");
exit(1);
}
pthread_create(&t1,NULL,funzione,(void*)clientsock);
}
exit(0);
}

void *funzione(void *arg){
int lun=0;
int sock=(int)arg;
char buff[250];
char nome[50];
int max;
int n;
pthread_detach(pthread_self());
while(1){
lun=read(sock,buff,250);
if(lun<0){
perror("ERRORE DURANTE LA REGISTRAZIONE\n");
exit(1);
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
n=atoi(&buff[5]);
if(n>max){
max=n;
}
n=max;
messaggio.valore=n;
pthread_mutex_unlock(&mutex);
sprintf(buff,"OK");
write(sock,buff,strlen(buff)+1);
}
if(strncmp(buff,"read",4)==0){
pthread_mutex_lock(&mutex);
sprintf(buff,"%d %s %d",messaggio.id,messaggio.nome,messaggio.valore);
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
